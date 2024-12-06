#include <string>
#include <iostream>
using namespace std;


enum status{ //Состояния процесса
	RUNNING, 
	WAITING,
	STOPPED
};

class PCB { // Класс для процесса
public:
	int process_id;
	string process_name;
	int process_status;
	int command_counter;
	int* cpu_registers;
	size_t num_of_cpu_r;
	PCB(){ //Конструктор по умолчанию. Ссылка занулена для того, чтобы не было ошибки при вызове деконструктора.
		cpu_registers = nullptr;
	}
	PCB(int process_id, string process_name, int process_status, int* cpu_registers, size_t num_of_cpu_r) { //Обычный конструктор
		this->process_id = process_id;
		this->process_name = process_name;
		this->process_status = process_status;
		this->command_counter = 0;
		this->num_of_cpu_r = num_of_cpu_r;
		this->cpu_registers = new int[this->num_of_cpu_r];
		for (size_t i = 0; i <= this->num_of_cpu_r - 1; ++i) {
			this->cpu_registers[i] = cpu_registers[i];
		}
	}
	PCB(const PCB& other, int command_counter) { //Конструктор копирования и присвоения номера команды
		this->process_id = other.process_id;
		this->process_name = other.process_name;
		this->process_status = other.process_status;
		this->command_counter = command_counter;
		this->num_of_cpu_r = other.num_of_cpu_r;
		this->cpu_registers = new int[this->num_of_cpu_r];
		for (size_t i = 0; i <= this->num_of_cpu_r - 1; ++i) {
			this->cpu_registers[i] = other.cpu_registers[i];
		}
	}
	~PCB(){ //Деконструктор
		delete[] cpu_registers;
	}

	void operator =(const PCB& other) { //Перегрузка оператора присвоения
		this->process_id = other.process_id;
		this->process_name = other.process_name;
		this->process_status = other.process_status;
		this->command_counter = other.command_counter;
		this->num_of_cpu_r = other.num_of_cpu_r;
		this->cpu_registers = new int[this->num_of_cpu_r];
		for (size_t i = 0; i <= this->num_of_cpu_r - 1; ++i) {
			this->cpu_registers[i] = other.cpu_registers[i];
		}
	}

	void print() { //Функция для вывода
		cout <<"Process id: " << process_id << endl;
		cout << "Process name: " << process_name << endl;
		cout << "Process status: " << process_status << endl;
		cout << "Next process id: " << command_counter << endl;
		cout << "Number of cpu registers: " << num_of_cpu_r << endl;
		cout << "Cpu registers: ";
		for (size_t i = 0; i <= num_of_cpu_r - 1; ++i) {
			cout << "[" << cpu_registers[i] << "] ";
		}
		cout << endl;
	}
};




class ProcessList { //Класс списка процессов
private:
	class ListNode { //Класс для узла. Он вложен в класс ProcessList, так как применяется только в нём 
	public:
		PCB data; //Процесс в узле
		ListNode* next; //Указатель на следующий узел

		ListNode(const PCB& newPCB, int number_of_command, ListNode* next) { //Конструктор узла
			this->next = next;
			data = newPCB;
			data.command_counter = number_of_command;
		}
		~ListNode() { //Деконструктор узла.
			delete next;
		}

	};

public:
	ListNode* head; //Указатель на первый узел списка

	ProcessList() { //Конструктор списка
		this->head = nullptr;
	}
	~ProcessList() { //Деконструктор. Удаление первого узла вызовет цепную реакцию удаления всех узлов
		delete this->head;
	}

	bool insert(const PCB& newPCB){ //Функция вставки
		if (head == nullptr) { //Если лист пустой, элемент вставится на первое место
			head = new ListNode(newPCB, 0, nullptr);
			return true;
		}
		else if (head->data.process_id == newPCB.process_id) return false; //Если первый элемент имеет такой же айди как и вставляемый, вставка не произойдёт
		else if (head->data.process_id > newPCB.process_id) { //Если первый элемент имеет айди больше, чем у вставляемого процесса, 
			ListNode* tmp_ptr = head;						  //этот процесс вставится на первое место
			head = new ListNode(newPCB, tmp_ptr->data.process_id, tmp_ptr);
			return true; 
		}
		else { //Ищутся такие два процесса, чтобы у одного айди был меньше чем у вставляемого, а у следующего - больше. 
			ListNode* ptr_ln = this->head; //После этого вставляемый элемент помещается между ними 
			while (true) {
				if (ptr_ln->next == nullptr) { // Если не нашлось процесса с большим айди, чем у вставляемого, то вставляемый процесс помещается в конец списка
					ptr_ln->next = new ListNode(newPCB, 0, nullptr);
					ptr_ln->data.command_counter = newPCB.process_id;
					return true;
				}
				ListNode* tmp_ptr_ln = ptr_ln->next; //ptr_ln -> tmp_ptr_ln
				if (tmp_ptr_ln->data.process_id == newPCB.process_id) return false; //Если найден процесc с таким же айди как у вставляемого, вставка не происходит
				else if (tmp_ptr_ln->data.process_id < newPCB.process_id) { //Тут происходит поиск описанный ранее
					ptr_ln = ptr_ln;
					continue;
				}
				else { //Это случай когда удалось найти ранее описанную ситуацию
					ptr_ln->next = new ListNode(newPCB, tmp_ptr_ln->data.process_id, tmp_ptr_ln);
					ptr_ln->data.command_counter = newPCB.process_id;
					return true;
				}
			}
		}
	}

	bool remove(int pid){
		if (head->data.process_id == pid) { //Если нужно удалить первый процесс
			ListNode* tmp = head->next;
			head->next = nullptr;
			delete head;
			head = tmp;
			return true;
		}
		else { // past -> checked -> after_checked
			ListNode* past = head;
			while (true) { //Поиск процесса с соответствующим айди
				if (past->next == nullptr) return false; //Случай когда список закончился, а процесс не был найден
				ListNode* checked = past->next;
				if (checked->data.process_id == pid) { //Если процесс найден
					ListNode* after_checked = checked->next;
					checked->next = nullptr;
					delete past->next;
					past->next = after_checked;
					if (after_checked != nullptr) {
						past->data.command_counter = after_checked->data.process_id;
					}
					else {
						past->data.command_counter = 0;
					}
					return true;
				}
				past = checked;
			}

		}
	}

	void print_list(){ //Вывод листа
		ListNode* ptr = head;
		while (ptr != nullptr) {
			ptr->data.print();
			cout << endl;
			ptr = ptr->next;
		}
	}
};

