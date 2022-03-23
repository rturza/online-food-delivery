#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

struct item
{
    int index, count;
    string name;
    double price;
    struct item *next;
}*menu_head = NULL; //head pointer of the linked list that holds the food menu

class customer
{
public:
    string name, address, phone;
    int order_count;
    double bill;
    item *ostack_top;
    customer *next; //points to the next customer in queue

    customer()
    {
        next = NULL;
        ostack_top = NULL;
        bill = 0;
        order_count = 0;
    }
    void push_order(int, int);
    item* pop_order();
}*cust_qfront = NULL, *cust_qrear = NULL; //the front & rear pointers of the customer queue


int menu_count = 0; //total number of items available
int cust_count = 0; //total number of customers in queue

void customer_menu();
void admin_menu();
void show_report();
void save_data();


void enqueue_customer(customer *newcust)
{
    if(cust_qfront == NULL)
    {
        cust_qfront = newcust;
        cust_qrear  = newcust;
    }
    else
    {
        cust_qrear->next = newcust;
        cust_qrear       = newcust;
    }
    cust_count++;
}

void dequeue_customer()
{
    customer *temp;

    if(cust_qfront == NULL) //no customers in queue
    {
        system("cls");
        cout<<"\nthere are no customers in queue!\n"<<endl;
    }
    else if(cust_count == 1) //only 1 customer in queue
    {
        temp = cust_qfront;
        cust_qfront = NULL;
        cust_qrear  = NULL;
        delete temp;
        cust_count--;
    }
    else
    {
        temp        = cust_qfront;
        cust_qfront = cust_qfront->next;
        delete temp;
        cust_count--;
    }
}

void list_cust()
{
    customer *temp = cust_qfront;
    int i = 1;
    while(temp != NULL)
    {
        cout<<i<<". "<<temp->name<<endl;
        temp = temp->next;
        i++;
    }
}

void customer::push_order(int index, int quantity)
{
    for(int loop = 0; loop < quantity; loop++)
    {
        struct item *temp1 = menu_head;
        struct item *temp2 = new item;

        order_count++;

        while(temp1 != NULL)
        {
            if(temp1->index == index)
            {
                temp2->index = temp1->index;
                temp2->name  = temp1->name;
                temp2->price = temp1->price;

                temp1->count++;
                bill = bill+temp1->price;
                break;
            }
            temp1 = temp1->next;
        }

        if(ostack_top == NULL)
        {
            temp2->next = NULL;
            ostack_top  = temp2;
        }
        else
        {
            temp2->next = ostack_top;
            ostack_top  = temp2;
        }
    }
}

item* customer::pop_order()
{
    struct item *temp;
    temp = ostack_top;

    if(temp != NULL)
    {
        system("cls");
        cout<<"You haven't placed any orders yet!\n"<<endl;
        return NULL;
    }
    else
    {
        temp = temp->next;

        struct item *pop = new item;
        pop = temp;

        bill = bill - pop->price; ///updating bill total

        delete temp;
        return pop;
    }
}

void insert_item(int index, string name, double price, int count)
{
    if(index < 0)
    {
        cout<<"Invalid index"<<endl;
        return;
    }

    int currIndex	=	1;
	item* currNode  =	menu_head;

    while (currNode!=NULL && index > currIndex) {
		currNode	=	currNode->next;
		currIndex++;
	}

	if (index > 0 && currNode == NULL)
    {
        cout<<"Invalid index"<<endl;
        return;
    }

	item* newNode = new item;

	newNode->index = index;
	newNode->name  = name;
	newNode->price = price;
	newNode->count = count;

	if (index == 0) {
		newNode->next	=	menu_head;
		menu_head		=	newNode;
	}
	else {
		newNode->next	=	currNode->next;
		currNode->next	=	newNode;
	}
}

void print_foodlist()
{
    struct item *temp = menu_head;

    cout<<"\nindex\t\t name\t\t price\t\n"<<endl;
    while(temp != NULL)
    {
        cout << " [" << temp->index+1 << "]\t";
        cout << "\t" << temp->name    << "\t";
        cout << "\ttk " << temp->price   << "\t";
        cout << endl;
        temp = temp->next;
    }
}

void process_customer() //processes the order of the customer at the front of the queue
                        //and writes his information in a separate .txt file
{
    system("cls");
    ofstream outfile;
    string filename;
    customer *cust = cust_qfront;

    filename = cust->name + "_" + cust->phone + ".txt";
    outfile.open(filename.c_str());

    outfile << "Name   : " << cust->name <<endl;
    outfile << "Address: " << cust->address <<endl;
    outfile << "Phone  : " << cust->phone <<endl<<endl;
    outfile << "Bill   :" <<endl;
    outfile<<endl;

    item *temp = cust->ostack_top;
    while(temp != NULL)
    {
        outfile<<temp->name<<"\ttk "<<temp->price<<endl;
        temp = temp->next;
    }
    outfile<<"\ntotal:\ttk "<<cust->bill;
    outfile<<endl;

    outfile.close();
    dequeue_customer();
    cout<<"\norders have been processed!"<<endl;
    system("pause");

    return;
}

void main_menu()
{
    int choice;
    system("cls");
    cout<<endl;
    cout<<"[1] customer access\n"<<endl;
    cout<<"[2] admin access\n"<<endl;
    cout<<"[0] exit\n\n";
    cout<<"your choice: ";
    cin>>choice;

    switch(choice)
    {
    case 1:
        system("cls");
        customer_menu();
        break;
    case 2:
        system("cls");
        admin_menu();
        break;
    case 0:
        system("cls");
        save_data();
        exit(1);
    }
}

void customer_menu()
{
    customer *cust = new customer;
    int choice;
    int quantity = 0;
    while(1)
    {
        system("cls");
        print_foodlist();
        cout<<"\nInput [index] to select an item\n"<<endl;
        cout<<"\nInput  0 to view your selection and total bill\n"<<endl;
        cout<<"\nInput -1 to cancel the last order\n"<<endl;
        cout<<"\nInput -2 to return to main menu\n"<<endl;
        cout<<"\nchoice: ";
        cin>>choice;
        cout<<endl;

        if(choice > 0 && choice <= menu_count)
        {
            system("cls");
            print_foodlist();
            cout<<"\nselected: item #"<<choice<<endl;
            cout<<"\nspecify quantity = ";
            cin>> quantity;
            cust->push_order(choice-1, quantity);
            cout<<"\norder(s) added\n"<<endl;
            system("pause");
        }
        else if(choice == 0)
        {
            if(cust->order_count == 0)
            {
                system("cls");
                cout<<"\nNo orders taken.\n"<<endl;
            }
            else
            {
                system("cls");

                item *temp;
                temp = cust->ostack_top;

                cout<<"Your bill:\n"<<endl;
                while(temp != NULL)
                {
                    cout<<temp->name<<"\ttk "<<temp->price<<endl;
                    temp = temp->next;
                }
                cout<<"\ntotal:\ttk "<<cust->bill;
                cout<<endl<<endl;

                cout<<"What would you like to do?\n"<<endl;
                cout<<"[1] Place more orders"<<endl;
                cout<<"[2] Confirm current orders"<<endl;
                cout<<"[0] Cancel all orders and return to main menu"<<endl;
                cout<<"\nYour choice:";

                int choice;
                cin>>choice;
                cout<<endl;

                if(choice == 1) continue;

                else if(choice == 2)
                {
                    system("cls");

                    cout<<"Name: ";
                    cin.ignore();
                    getline(std::cin, cust->name);
                    cout<<endl;

                    cout<<"Address: ";
                    getline(std::cin, cust->address);
                    cout<<endl;

                    cout<<"Phone no.: ";
                    getline(std::cin, cust->phone);
                    cout<<endl;

                    enqueue_customer(cust); ///enqueue customer

                    cout<<"\norders confirmed!\n"<<endl;

                    system("pause");
                    return;
                }

                else if(choice == 0)
                {
                    system("cls");
                    cout<<"\norders canceled!\n"<<endl;
                    system("pause");
                    return;
                }

                else
                {
                    cout<<"\ninvalid selection\n"<<endl;
                }
            }

            system("pause");
        }
        else if(choice == -1)
        {
            struct item *pop;
            pop = cust->pop_order();
            if(pop != NULL) cout<<"\nitem removed: "<<pop->name<<endl<<endl;
            system("pause");
        }
        else if(choice == -2)
        {
            system("cls");
            return;
        }
        else
        {
            system("cls");
            cout<<"\nInvalid selection\n"<<endl;
            system("pause");
        }
    }
    return;
}

void admin_menu()
{
    while(1)
    {
        system("cls");
        cout<<"\n[1] show report\n"<<endl;
        cout<<"\n[2] show total number of customers in queue\n"<<endl;
        cout<<"\n[3] process customer\n"<<endl;
        cout<<"\n[0] return to main menu\n"<<endl;
        cout<<"\nyour choice: ";
        int choice;
        cin>>choice;

        if(choice == 1) show_report();

        else if(choice == 2)
        {
            system("cls");
            cout<<"\n\ntotal number of customers in queue:" << cust_count<<endl;
            cout<<endl;
            if (cust_count > 0 )
            {
                cout<<"\nCustomers in queue:\n"<<endl;
                list_cust();
            }
            system("pause");
        }
        else if(choice == 3 && cust_count > 0) process_customer(); //processes the order of the customer at the front of the queue
        else if(choice == 3 && cust_count == 0)
        {
            system("cls");
            cout<<"\nerror: there are no customers in queue to process!\n"<<endl;
            system("pause");
        }
        else if(choice == 0) return;

        else
        {
            cout<<"\ninvalid selection"<<endl;
            system("pause");
        }
    }
}

void show_report()
{
    item *temp = menu_head;

    cout<<"\n\titem"<<"\ttotal quantities sold\n"<<endl;

    while(temp != NULL)
    {
        cout<<"\t"<<temp->name<<"\t\t"<<temp->count<<endl;
        temp = temp->next;
    }
    cout<<endl;
    system("pause");
}

void load_data()
{
    ifstream infile("menu.txt");
    if(infile.is_open())
    {
        int index, count;
        string name;
        double price;

        infile >> index >> name >> price >> count;
        while(true)
        {
            insert_item(index, name, price, count);
            menu_count++;
            infile >> index >> name >> price >> count;
            if(infile.eof()) break;
        }
        infile.close();
    }
}

void save_data()
{
    ofstream outfile("menu.txt");
    item *temp = menu_head;
    if(outfile.is_open())
    {
        while(temp != NULL)
        {
            outfile<<temp->index<<" "<<temp->name<<" "<<temp->price<<" "<<temp->count<< endl;
            temp = temp->next;
        }
        outfile.close();
    }
}

int main()
{
    load_data();
    while(true) main_menu();
    return 0;
}
