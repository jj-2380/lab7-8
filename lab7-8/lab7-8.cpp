#include "classes.h"


int main() {
	setlocale(LC_ALL, "ru");

	ProcessList List; //Объявление

	int lst1[3] = { 2334, 3454 ,234 };
	int lst2[4] = { 8703, 75 ,534, 345 };
	int lst3[2] = { 123, 354 };
	int lst4[5] = { 1, 2, 3, 4, 5 };

	//Добавление
	List.insert(PCB(12, (string)"Download", STOPPED, lst1, 3));
	List.insert(PCB(32, (string)"Calculation", RUNNING, lst2, 4));
	List.insert(PCB(4, (string)"Processing", WAITING, lst3, 2));
	List.insert(PCB(7, (string)"Dynamics", WAITING, lst4, 5));

	//Вывод
	cout << "Первый вывод:" << endl;
	List.print_list();
	cout << endl;

	//Удаление
	List.remove(7);
	List.remove(32);

	//Вывод
	cout << "Вывод после удаления:" << endl;
	List.print_list();
	cout << endl;

	//Попытки неудачных действий
	cout << "Попытки неудачных действий:" << endl;
	cout << "Попытка вставки: " << List.insert(PCB(12, (string)"fourth", WAITING, lst4, 5)) << endl;
	cout << "Попытка удаления: " << List.remove(52) << endl; 
	cout << endl;

	//Финальный вывод
	cout << "Финальный вывод" << endl;
	List.print_list();

	return 0;
}