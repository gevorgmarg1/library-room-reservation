#ifndef __RESERVATION_H__
#define __RESERVATION_H__


class Reservation 
{
    private:
        int start, end, day, month, roomID, reservationID;
        static int lastIncrementID;

    public:
        Reservation()
        {
            start = 0;
            end = 0;
            day = 0;
            month = 0;
            roomID = 0;
            reservationID = 0;
        }
        Reservation(int start, int end, int day, int month, int roomID) : start(start), end(end), day(day), month(month), roomID(roomID)
        {
            reservationID = ++lastIncrementID;
        }
        void setInfo(int s, int e, int d, int m, int r, int i)
        {
            start = s;
            end = e;
            day = d;
            month = m;
            roomID = r;
            reservationID = i;
            lastIncrementID = i;
        }
        int getRoom(){ return roomID; }
        int getStart(){ return start; }
        int getEnd(){ return end; }
        int getMonth(){ return month; }
        int getDay(){ return day; }
        int getID(){ return reservationID; }
        void print(ostream &os)
        {
            os<<"Reservation: "<<reservationID<<endl;
            os << "\t";
            os<<"Day: "<<month<<"/"<<day<<endl;
            os << "\t";
            os<<"Time: "<<start<<":00 to "<<end<<":00"<<endl;
        }
};
int Reservation :: lastIncrementID = 1000;
#endif
