#ifndef __CLASSROOM_H__
#define __CLASSROOM_H__

class Classroom
{
    private:
        int id;
        int number;
        string building;
        int capacity;
    public:
        Reservation** reservations;
        Classroom()
        {
            id = 0;
            number = 0;
            building = "";
            capacity = 0;
            reservations = 0;
        }
        Classroom(int idnum, int num, string bld, int cap)
        {
            id = idnum;
            number = num;
            building = bld;
            capacity = cap;
            reservations = 0;
        }
        void setInfo(int i, int n, string b, int c)
        {
            id = i;
            number = n;
            building = b;
            capacity = c;
            reservations = 0;
        }
        int getID(){ return id; }
        int getNumber(){ return number; }
        string getBuilding(){ return building; }
        int getCapacity(){ return capacity; }
        void print(bool showReservations, ostream &os)
        {
            // cout<<"ID: "<<id<<endl;
            os<<"Room: "<<building<<" "<<number<<endl;
            os<<"Capacity: "<<capacity<<endl;
            os<<endl;
            if(reservations && showReservations)
            {
            os<<"Reservations:"<<endl;
            int len = 0;
            while(reservations[++len]){}
            for(int i = 0; i<len;i++)
            {
                reservations[i]->print(os);
            }
            }
        }
        void addReservation(Reservation* rsrvation)
        {
            if (reservations)
            {
                int len = 0;
                while (reservations[len] != nullptr)
                {
                    len++;
                }

                Reservation** temp = new Reservation*[len + 2];
                for (int i = 0; i < len; i++)
                {
                    temp[i] = reservations[i];
                }
                temp[len] = rsrvation;
                temp[len + 1] = nullptr;

                delete[] reservations;
                reservations = temp;
            }
            else
            {
                reservations = new Reservation*[2];
                reservations[0] = rsrvation;
                reservations[1] = nullptr;
            }
        }
         ~Classroom() 
        {
            if(reservations)
            {
                delete[] reservations;
            }
        }
        void showAvailability(int day, int month, ostream &os)
        {
            const int start = 9;
            const int end = 21;
            bool availability[end - start] = { true };

            // Initialize all slots to true
            for (int i = 0; i < end - start; i++) {
                availability[i] = true;
            }

            // Loop through reservations and mark as false if a reservation exists for the given day and month
            for (int i = 0; i < 10 && reservations[i] != nullptr; i++) {
                if (reservations[i]->getDay() == day && reservations[i]->getMonth() == month) {
                    for (int j = reservations[i]->getStart(); j < reservations[i]->getEnd(); j++) {
                        if (j >= start && j < end) {
                            availability[j - start] = false;
                        }
                    }
                }
            }

            os << "Room " << number << " in " << building << " - Availability for " << month << "/" << day << ":\n";
            for (int i = 0; i < end - start; i++) {
                os << start + i << ":00 - " << start + i + 1 << ":00: " 
                    << (availability[i] ? "Available" : "Reserved") << '\n';
            }
        }
};

#endif
