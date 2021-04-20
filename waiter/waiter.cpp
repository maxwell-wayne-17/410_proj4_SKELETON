#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	return this->myIO.getNext(anOrder);
}

void Waiter::beWaiter() {

	//waiter is starting
	b_WaiterIsFinished = false;

	while(!b_WaiterIsFinished){
		unique_lock<mutex> lck(mutex_order_inQ);
		//get order from file, add if success, otherwise waiter done
		ORDER newOrder;
		if ( getNext(newOrder) == SUCCESS){
			order_in_Q.push(newOrder);
			cv_order_inQ.notify_all();
		}
		else{
			b_WaiterIsFinished = true;
			cv_order_inQ.notify_all();
		}


	}

}

