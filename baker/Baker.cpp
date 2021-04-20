#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

void Baker::bake_and_box(ORDER &anOrder) {

	//get number of full boxes needed
	int numBoxes = anOrder.number_donuts / 12;
	for (int i = 0; i < numBoxes; i++){

		//create box to put in vector
		Box newBox = Box();

		for (int j = 0; j < 12; j++){

			//fill current box
			DONUT newDonut;
			newBox.addDonut(newDonut);
		}
		//push filled box into vector
		anOrder.boxes.push_back(newBox);

	}
	//add leftover donuts in one box
	int leftDonuts = anOrder.number_donuts % 12;
	if (leftDonuts % 12 != 0){
		Box newBox = Box();
		for (int i = 0; i < leftDonuts % 12; i++){
			DONUT newDonut;
			newBox.addDonut(newDonut);
		}
		//push leftover box into vector
		anOrder.boxes.push_back(newBox);
	}
}

void Baker::beBaker() {
	{
		unique_lock<mutex> lck(mutex_order_inQ);
		while (!b_WaiterIsFinished){
			cv_order_inQ.wait(lck);
		}
	}
	
	bool run = true;
	while (run) {

		unique_lock<mutex> lck(mutex_order_inQ);
		//if inQ is empty and waiter is done then break
		if ( order_in_Q.empty() && b_WaiterIsFinished ){
			run = false;
		}

		//if inQ is empty but waiter is not done, then wait
		while ( order_in_Q.empty() && !b_WaiterIsFinished ){
			cv_order_inQ.wait(lck);
		}


		//make boxes
		if ( !order_in_Q.empty() ){

			ORDER currentOrder = order_in_Q.front();
			this->bake_and_box( currentOrder );
			order_in_Q.pop();
			//push into out vector
			order_out_Vector.push_back(currentOrder);

		}

	}

}
