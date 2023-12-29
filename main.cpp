#include<iostream>
#include<fstream>
#include<sstream>
#include"/usr/local/cs/cs251/react.h"
using namespace std;

#include "Reservation.h"
#include "Classroom.h"
extern Classroom allClassrooms[];
extern Reservation allReservations[];
extern int num_lines_roomsDB;
extern int num_lines_reservationsDB;

class State {
    int screen_num;
    int reserveRoomInputs[5];
    int findRoomInputs[5];
    int showAvailabilityInputs[3];
    int editReservationInputs[6];
public:
    State(istream &is)
    {
        is >> screen_num;
        for(int i = 0; i < 5; i++)
        {
            is >> reserveRoomInputs[i];
        }
        for(int i = 0; i < 5; i++)
        {
            is >> findRoomInputs[i];
        }
        for(int i = 0; i < 3; i++)
        {
            is >> showAvailabilityInputs[i];
        }
        for(int i = 0; i < 6; i++)
        {
            is >> editReservationInputs[i];
        }
    }
    int get_screen_num() { return screen_num; }
    int* get_reserveRoomInputs() { return reserveRoomInputs; }
    int* get_findRoomInputs() { return findRoomInputs; }
    int* get_showAvailabilityInputs() { return showAvailabilityInputs; }
    int* get_editReservationInputs() { return editReservationInputs; }
    void write_to(ostream &os)  
    {
        os << screen_num << endl;
        for(int i = 0; i < 5; i++)
        {
            os << reserveRoomInputs[i] << endl;
        }
        for(int i = 0; i < 5; i++)
        {
            os << findRoomInputs[i] << endl;
        }
        for(int i = 0; i < 3; i++)
        {
            os << showAvailabilityInputs[i] << endl;
        }
        for(int i = 0; i < 6; i++)
        {
            os << editReservationInputs[i] << endl;
        }
    }
    void update(string input, istream &is);
};

//global variables and functions
class DatabaseManager {
public:
    DatabaseManager() {
        readRoomsDB("roomsDB.txt");
        readReservationsDB("reservationsDB.txt");
    }

    ~DatabaseManager() {
        writeRoomsDB("roomsDB.txt");
        writeReservationsDB("reservationsDB.txt");
    }
private:
    void readRoomsDB(const string& filename) {
    ifstream roomsDB(filename);
    if (!roomsDB) {
        cerr << "Failed to open " << filename << " for reading." << endl;
        return;
    }
    num_lines_roomsDB = countLines(filename);

    for (int i = 0; i < num_lines_roomsDB; i++) {
        int roomID, roomNUM, roomCAP;
        string building;
        if (roomsDB >> roomID >> building >> roomNUM >> roomCAP) {
            allClassrooms[i].setInfo(roomID, roomNUM, building, roomCAP);
        } else {
            cerr << "Invalid data format in " << filename << endl;
            break; // or handle the error as needed
        }
    }
}

void readReservationsDB(const string& filename) {
    ifstream reservationsDB(filename);
    if (!reservationsDB) {
        cerr << "Failed to open " << filename << " for reading." << endl;
        return;
    }
    num_lines_reservationsDB = countLines(filename);

    for (int i = 0; i < num_lines_reservationsDB; i++) {
        int RSV_roomID, reservationID;
        string RSV_data;
        if (reservationsDB >> reservationID >> RSV_roomID >> RSV_data) {
            stringstream ss(RSV_data);
            int RSV_data_int[4];
            int day, month, time_start, time_end;
            int k = 0;
            string str;
            while (getline(ss, str, '-')) {
                RSV_data_int[k] = stoi(str);
                k++;
            }
            day = RSV_data_int[3];
            month = RSV_data_int[2];
            time_end = RSV_data_int[1];
            time_start = RSV_data_int[0];

            allReservations[i].setInfo(time_start, time_end, day, month, RSV_roomID, reservationID);
        } else {
            cerr << "Invalid data format in " << filename << endl;
            break; // or handle the error as needed
        }
    }

    // Pointing rooms to reservations
    for (int m = 0; m < num_lines_roomsDB; m++) {
        for (int n = 0; n < num_lines_reservationsDB; n++) {
            if (allReservations[n].getRoom() == allClassrooms[m].getID()) {
                allClassrooms[m].addReservation(&allReservations[n]);
            }
        }
    }
}

    void writeRoomsDB(const string& filename) {
        ofstream write_roomsDB(filename);
        for (int i = 0; i < num_lines_roomsDB; i++) {
            write_roomsDB << allClassrooms[i].getID() << " " << allClassrooms[i].getBuilding() << " " << allClassrooms[i].getNumber() << " " << allClassrooms[i].getCapacity() << endl;
        }
    }

    void writeReservationsDB(const string& filename) {
        ofstream write_reservationsDB(filename);
        for (int i = 0; i < num_lines_reservationsDB; i++) {
            write_reservationsDB << allReservations[i].getID() << " " << allReservations[i].getRoom() << " " << allReservations[i].getStart() << "-" << allReservations[i].getEnd() << "-" << allReservations[i].getMonth() << "-" << allReservations[i].getDay() << endl;
        }
    }

    int countLines(const string& filename) {
        ifstream file(filename);
        int lines = 0;
        string line;
        while (getline(file, line)) {
            lines++;
        }
        return lines;
    }
};



// ------------ GLOBAL VARIABLES ------------
    const int text_lines_limit = 31;
    string text[text_lines_limit];
    int screen_num = 0;
    const int limit = 31;  // final screens

    const int maxRooms = 100;
    Classroom allClassrooms[maxRooms];
    int num_lines_roomsDB = 0;

    const int maxReservations = 100;
    Reservation allReservations[maxReservations];
    int num_lines_reservationsDB = 0;
    DatabaseManager dbManager;

// ------------------------------------

// ------------ COUNT FILE LINES ------------
    int countLines(string fileName)
    {
        ifstream file(fileName);
        int lines = 0;
        string line;
        while(getline(file, line))
        {
            lines++;
        }
        return lines;
    }
// ------------------------------------

// ------------ CREATING NEW RESERVATION ------------
    int create_reservation(int roomNumber, int day,  int month, int startTime, int endTime)
    {
            string building = "RML";

            int roomID = -1;
            for (int i = 0; i < num_lines_roomsDB; ++i) {
                if (allClassrooms[i].getNumber() == roomNumber && allClassrooms[i].getBuilding() == building) {
                    roomID = allClassrooms[i].getID();
                    break;
                }
            }

            if (roomID == -1) {
                return 0;
            } else {
                // Creating a new reservation
                Reservation newReservation(startTime, endTime, day, month, roomID);

                // Adding reservation to the array (if possible)
                if (num_lines_reservationsDB < maxReservations) {
                    allReservations[num_lines_reservationsDB] = newReservation;
                    num_lines_reservationsDB++;
                } else {
                    return 0;
                }

                // Adding reservation to the classroom
                for (int i = 0; i < num_lines_roomsDB; ++i) {
                    if (allClassrooms[i].getID() == roomID) {
                        allClassrooms[i].addReservation(&allReservations[num_lines_reservationsDB - 1]);
                        break;
                    }
                }
                return 1;
            }
    }
// ------------------------------------

// ------------ FINDING ROOM ------------
void findingRoom(int month, int day, int time_start, int time_end, int capacity, ostream &os) 
{
    for (int i = 0; i < num_lines_roomsDB; i++) 
    {
        if (allClassrooms[i].getCapacity() >= capacity) 
        {
            bool checkAvailability = true;
            for (int j = 0; j < num_lines_reservationsDB; j++) 
            {
                if (allReservations[j].getRoom() == allClassrooms[i].getID() && 
                    allReservations[j].getDay() == day && 
                    allReservations[j].getMonth() == month) 
                {
                        if (!(time_end <= allReservations[j].getStart() || time_start >= allReservations[j].getEnd()))
                        {
                            checkAvailability = false;
                            break;
                        }
                }
            }
            if (checkAvailability) 
            {
                os << allClassrooms[i].getBuilding() << " " << allClassrooms[i].getNumber() << endl;
                allClassrooms[i].print(false, os);
            }
        }
    }
}
// ------------------------------------

// ------------ SHOW AVAILABILITY ------------
void showAvailability(int room, int month, int day, ostream &os) 
{
    int roomNumber = 0;
    for(int i = 0; i<num_lines_roomsDB; i++)
    {
        if(allClassrooms[i].getNumber() == room)
        {
            roomNumber = i;
            break;
        }
    }
    allClassrooms[roomNumber].showAvailability(day, month, os);
}


// ------------------------------------

// ------------ SHOW RESERVATIONS ------------
void showReservations(ostream &os)
{
    for(int i = 0; i<num_lines_reservationsDB; i++)
    {
        for(int k = 0; k<num_lines_roomsDB; k++)
        {
            if(allReservations[i].getRoom() == allClassrooms[k].getID())
            {
                os<<allClassrooms[k].getBuilding()<<" "<<allClassrooms[k].getNumber()<<endl;
                allReservations[i].print(os);
            }
        }
    }
}
// ------------------------------------

// ------------ CHECK AVAILABILITY ------------
bool checkAvailability(int reservationID=0, int roomID=0,  int month=0, int day=0, int time_start=0, int time_end=0)
{
    if(roomID == 0)
    {
        return false;
    }
    for(int i = 0; i<num_lines_reservationsDB; i++)
    {
        if(reservationID != allReservations[i].getID())
        {
            if(roomID == allReservations[i].getRoom())
            {
                if(allReservations[i].getMonth() == month && allReservations[i].getDay() == day)
                {
                    if((time_start >= allReservations[i].getStart() && time_start < allReservations[i].getEnd()) || (time_end > allReservations[i].getStart() && time_end < allReservations[i].getEnd()))
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
// ------------------------------------

// ------------ Edit Reservation ------------
bool editReservation(int reservationID, int newRoomNumber, int newMonth, int newDay, int newStartTime, int newEndTime) {
    // Find the reservation by ID
    Reservation* reservationToEdit = nullptr;
    for (int i = 0; i < num_lines_reservationsDB; i++) {
        if (allReservations[i].getID() == reservationID) {
            reservationToEdit = &allReservations[i];
            break;
        }
    }

    if (reservationToEdit == nullptr) {
        return false;
    }

    int roomID = 0;
    for(int i = 0; i<num_lines_roomsDB;i++)
    {
        if(allClassrooms[i].getNumber() == newRoomNumber)
        {
            roomID = allClassrooms[i].getID();
        }
    }
    // Check if the new details are available
    if (checkAvailability(reservationID, roomID, newMonth, newDay, newStartTime, newEndTime)) {
        // Update the reservation details
        reservationToEdit->setInfo(newStartTime, newEndTime, newDay, newMonth, roomID, reservationID);
        return true;
    } else {
        return false; }
}


// Global React-related variables
const int input_index = 2, prompt_index = 25;
bool react = false;

void read_text_data() 
    {
        ifstream fs("response.txt");
        string comment;
        for (int i = 0; i < text_lines_limit; ++i)
        {
            getline(fs, text[i], '#');
            text[i].pop_back();
            getline(fs, comment);
        }
        num_lines_roomsDB = countLines("roomsDB.txt");
        num_lines_reservationsDB = countLines("reservationsDB.txt");
    }


void display(State &state, ostream &os) {
    int screen_num = state.get_screen_num();
    if (react)
    {
        if(screen_num == 2 || screen_num == 1)
        {
            char button_labels[] = "Welcome to PRESSERS\nLibrary Room Reservation System\0Find Room\0Show Availability\0Create Reservation\0Show My Reservations\0Edit A Reservation";
            int button_labels_len = 138;
            _add_yaml("home.yaml");
            os.write(button_labels, button_labels_len);
            os << "~~~~~~~~~~~~~~~~~~~~~~~~~";  // filler
        }
        else if(screen_num == 3)
        {
            _add_yaml("findRoom.yaml");
            char button1_labels[] = "Find Room\0";
            int button1_labels_len = 10;
            os.write(button1_labels, button1_labels_len);
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            char button2_labels[] = "Submit\0";
            int button2_labels_len = 7;
            os.write(button2_labels, button2_labels_len);
            char button3_labels[] = "Return\0";
            int button3_labels_len = 7;
            os.write(button3_labels, button3_labels_len);
        }
        else if(screen_num == 9)
        {
            _add_yaml("showAvailability.yaml");
            char button1_labels[] = "Show Availability\0";
            int button1_labels_len = 18;
            os.write(button1_labels, button1_labels_len);
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            char button2_labels[] = "Submit\0";
            int button2_labels_len = 7;
            os.write(button2_labels, button2_labels_len);
            char button3_labels[] = "Return\0";
            int button3_labels_len = 7;
            os.write(button3_labels, button3_labels_len);
        }
        else if(screen_num == 13)
        {
            _add_yaml("createReservation.yaml");
            char button1_labels[] = "Create Reservation\0";
            int button1_labels_len = 19;
            os.write(button1_labels, button1_labels_len);
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            char button2_labels[] = "Submit\0";
            int button2_labels_len = 7;
            os.write(button2_labels, button2_labels_len);
            char button3_labels[] = "Return\0";
            int button3_labels_len = 7;
            os.write(button3_labels, button3_labels_len);
        }
        else if(screen_num == 23)
        {
            _add_yaml("editReservation.yaml");
            char button1_labels[] = "Edit Reservation\0";
            int button1_labels_len = 17;
            os.write(button1_labels, button1_labels_len);
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            os << "~~~~~~~~~~~~~~~~~~~~";  // filler
            char button2_labels[] = "Submit\0";
            int button2_labels_len = 7;
            os.write(button2_labels, button2_labels_len);
            char button3_labels[] = "Return\0";
            int button3_labels_len = 7;
            os.write(button3_labels, button3_labels_len);
        }
        else if(screen_num == 8)
        {
            char button1_labels[] = "Available rooms\0";
            int button1_labels_len = 16;
            char button2_labels[] = "Return\0";
            int button2_labels_len = 7;
            
            _add_yaml("result.yaml", {{"header", 0}, {"button", button1_labels_len}, {"text", button1_labels_len+button2_labels_len}});
            os.write(button1_labels, button1_labels_len);
            os.write(button2_labels, button2_labels_len);

            int* findRoomInputs = state.get_findRoomInputs();
            findingRoom(findRoomInputs[0], findRoomInputs[1], findRoomInputs[2], findRoomInputs[3], findRoomInputs[4], os);
        }
        else if(screen_num == 12)
        {
            int* showAvailabilityInputs = state.get_showAvailabilityInputs();

            char button1_labels[] = "Availability of a room\0";
            int button1_labels_len = 23;
            char button2_labels[] = "Return\0";
            int button2_labels_len = 7;
            
            _add_yaml("result.yaml", {{"header", 0}, {"button", button1_labels_len}, {"text", button1_labels_len+button2_labels_len}});
            os.write(button1_labels, button1_labels_len);
            os.write(button2_labels, button2_labels_len);

            
            showAvailability(showAvailabilityInputs[0], showAvailabilityInputs[1], showAvailabilityInputs[2], os);
        }
        else if(screen_num == 19)
        {
            char button1_labels[] = "Reservation Created Successfully!!!\0";
            int button1_labels_len = 36;
            char button12_labels[] = "Error has been occured!\nRoom not available for mentioned time!!!\0";
            int button12_labels_len = 65;

            char button2_labels[] = "Return\0";
            int button2_labels_len = 7;

            int* reserveRoomInputs = state.get_reserveRoomInputs();
            if(create_reservation(reserveRoomInputs[0], reserveRoomInputs[1], reserveRoomInputs[2], reserveRoomInputs[3], reserveRoomInputs[4]))
            {
                _add_yaml("result.yaml", {{"header", 0}, {"button", button1_labels_len}, {"text", button1_labels_len+button2_labels_len}});
                os.write(button1_labels, button1_labels_len);
            }
            else
            {
                _add_yaml("result.yaml", {{"header", 0}, {"button", button12_labels_len}, {"text", button12_labels_len+button2_labels_len}});
                os.write(button12_labels, button12_labels_len);
            }
            
            os.write(button2_labels, button2_labels_len);
        }
        else if(screen_num == 21)
        {
            char button1_labels[] = "My Reservations\0";
            int button1_labels_len = 16;
            char button2_labels[] = "Return\0";
            int button2_labels_len = 7;
            
            _add_yaml("result.yaml", {{"header", 0}, {"button", button1_labels_len}, {"text", button1_labels_len+button2_labels_len}});
            os.write(button1_labels, button1_labels_len);
            os.write(button2_labels, button2_labels_len);

            showReservations(os);
        }
        else if(screen_num == 30)
        {
            char button1_labels[] = "You have Successfully updated the reservation!\0";
            int button1_labels_len = 47;
            char button12_labels[] = "Wrong Reservation ID or the room is not avaialble at the given time!\0";
            int button12_labels_len = 69;
            char button2_labels[] = "Return\0";
            int button2_labels_len = 7;
            
            int* editReservationInputs = state.get_editReservationInputs();
            if(editReservation(editReservationInputs[0], editReservationInputs[1], editReservationInputs[2], editReservationInputs[3], editReservationInputs[4], editReservationInputs[5]))
            {
                _add_yaml("result.yaml", {{"header", 0}, {"button", button1_labels_len}, {"text", button1_labels_len+button2_labels_len}});
                os.write(button1_labels, button1_labels_len);
            }
            else
            {
                _add_yaml("result.yaml", {{"header", 0}, {"button", button12_labels_len}, {"text", button12_labels_len+button2_labels_len}});
                os.write(button12_labels, button12_labels_len);
            }
            
            os.write(button2_labels, button2_labels_len);
        }
        else if(screen_num == 31)
        {
            char button1_labels[] = "Wrong Reservation ID or the room is not avaialble at the given time!\0";
            int button1_labels_len = 69;
            char button2_labels[] = "Return\0";
            int button2_labels_len = 7;
            
            _add_yaml("result.yaml", {{"header", 0}, {"button", button1_labels_len}, {"text", button1_labels_len+button2_labels_len}});
            os.write(button1_labels, button1_labels_len);
            os.write(button2_labels, button2_labels_len);
        }
        else
        {
            _add_yaml("story.yaml", {{"prompt_index", prompt_index}, {"input_index", input_index}});
            os << "~~~~~~~~~~~~~~~~~~~~~~~~~";  // filler
        }
        
    } else {
        os << "\033";
    }
    
    if(!react)
    {
        if(screen_num == 1)
        {
            os << "\033"
            << "[screen " << screen_num << "]" << endl
            << text[screen_num];
        }
        else if(screen_num == 18)
        {
            int* reserveRoomInputs = state.get_reserveRoomInputs();
            if(create_reservation(reserveRoomInputs[0], reserveRoomInputs[1], reserveRoomInputs[2], reserveRoomInputs[3], reserveRoomInputs[4]))
            {
                // screen_num = 19;
                os << "\033"
                << "[screen " << 19 << "]" << endl
                << text[19];
                os << "[screen " << 22 << "]" << endl
                << text[22];
            }
            else
            {
                // screen_num = 20;
                os << "\033"
                << "[screen " << 20 << "]" << endl
                << text[20];
                os << "[screen " << 22 << "]" << endl
                << text[22];
            }
            screen_num = 22;
        }
        else if(screen_num == 8)
        {
            int* findRoomInputs = state.get_findRoomInputs();
            os << "\033"
                << "[screen " << 8 << "]" << endl
                << text[8];
            findingRoom(findRoomInputs[0], findRoomInputs[1], findRoomInputs[2], findRoomInputs[3], findRoomInputs[4], os);
            os << "[screen " << 22 << "]" << endl
                << text[22];
            screen_num = 22;
        }
        else if(screen_num == 12)
        {
            int* showAvailabilityInputs = state.get_showAvailabilityInputs();
            os << "\033"
                << "[screen " << 12 << "]" << endl
                << text[12];
            showAvailability(showAvailabilityInputs[0], showAvailabilityInputs[1], showAvailabilityInputs[2], os);
            os << "[screen " << 22 << "]" << endl
                << text[22];
            screen_num = 22;
        }
        else if(screen_num == 21)
        {
            os << "\033"
                << "[screen " << 21 << "]" << endl
                << text[21];
            showReservations(os);
            os << "[screen " << 22 << "]" << endl
                << text[22];
            screen_num = 22;
            
        }
        else if(screen_num == 29)
        {
            int* editReservationInputs = state.get_editReservationInputs();
            if(editReservation(editReservationInputs[0], editReservationInputs[1], editReservationInputs[2], editReservationInputs[3], editReservationInputs[4], editReservationInputs[5]))
            {
                os << "\033"
                << "[screen " << 30 << "]" << endl
                << text[30];
                os << "[screen " << 22 << "]" << endl
                << text[22];
            }
            else
            {
                os << "\033"
                << "[screen " << 31 << "]" << endl
                << text[31];
                os << "[screen " << 22 << "]" << endl
                << text[22];
            }
            screen_num = 22;
        }
        else
        {
            os << "\033"
            << "[screen " << screen_num << "]" << endl
            << text[screen_num];
        }
    }


    if (react) os << '\0'; //null
    else if (screen_num >= limit) exit(0);
}

    //how to adjust it???


void State::update(string input, istream &is) 
{
        int input_num;
        bool event = false;
        if (react && _received_event()) 
        {
            if (_event_id_is("return")){ screen_num = 2; event = true;}
            if (_event_id_is("find_room")){ input_num = 1; event = true;}
            if (_event_id_is("show_availability")){ input_num = 2; event = true;}
            if (_event_id_is("create_reservation")){ input_num = 3; event = true;}
            if (_event_id_is("show_reservations")){ input_num = 4; event = true;}
            if (_event_id_is("edit_reservation")){ input_num = 5; event = true;}
            if (_event_id_is("findRoom"))
            {
                is.get(); is.get();is.get(); is.get();is.get(); is.get();is.get(); is.get();is.get(); is.get();
                event = true;
                int i = 0;
                while(getline(is, input, '~') && i < 5)
                {
                    if (!input.empty())
                    {
                        findRoomInputs[i] = stoi(input);
                        i++;
                    }
                }
                screen_num = 8;
            }
            else if (_event_id_is("showAvailability"))
            {
                is.get(); is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();
                event = true;
                int i = 0;
                cout<<endl;
                while(getline(is, input, '~') && i < 3)
                {
                    if (!input.empty())
                    {
                        showAvailabilityInputs[i] = stoi(input);
                        i++;
                    }
                }
                screen_num = 12;
            }
            else if (_event_id_is("createReservation"))
            {
                is.get(); is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();
                event = true;
                int i = 0;
                cout<<endl;
                while(getline(is, input, '~') && i < 5)
                {
                    if (!input.empty())
                    {
                        reserveRoomInputs[i] = stoi(input);
                        i++;
                    }
                }
                screen_num = 19;
            }
            else if (_event_id_is("editReservation"))
            {
                is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();is.get();
                event = true;
                int i = 0;
                cout<<endl;
                while(getline(is, input, '~') && i < 6)
                {
                    if (!input.empty())
                    {
                        editReservationInputs[i] = stoi(input);
                        i++;
                    }
                }
                screen_num = 30;
            }
            else
            {
                is.get(); is.get(); // Skip first two characters
                if ('~' != is.peek()) getline(is, input, '\0');
            }

        }
        if (0 == screen_num) screen_num = 1;
        else if (1 == screen_num) screen_num = 2;
        else if (22 == screen_num) screen_num = 2;
        else if(input == "" && !event) screen_num = 2;
        else
        {
            if(!event) input_num = stoi(input);
            if (2 == screen_num) 
            {
                if (1 == input_num) screen_num = 3;
                else if (2 == input_num) screen_num = 9;
                else if (3 == input_num) screen_num = 13; 
                else if (4 == input_num) screen_num = 21; 
                else if (5 == input_num) screen_num = 23; 
                else if (text_lines_limit == input_num) screen_num = text_lines_limit; 
            }
            else if (screen_num == 3) {
                findRoomInputs[0] = input_num;
                screen_num = 4;
            } else if (screen_num == 4) {
                findRoomInputs[1] = input_num;
                screen_num = 5;
            } else if (screen_num == 5) {
                findRoomInputs[2] = input_num;
                screen_num = 6;
            } else if (screen_num == 6) {
                findRoomInputs[3] = input_num;
                screen_num = 7;
            } else if (screen_num == 7) {
                findRoomInputs[4] = input_num;
                screen_num = 8;
            }
            else if (screen_num == 9) {
                showAvailabilityInputs[0] = input_num;
                screen_num = 10;
            } else if (screen_num == 10) {
                showAvailabilityInputs[1] = input_num;
                screen_num = 11;
            } else if (screen_num == 11) {
                showAvailabilityInputs[2] = input_num;
                screen_num = 12;
            }
            // else if(12 == screen_num)
            // {
            //     screen_num = 20;
            // }
            else if (screen_num == 13) {
                reserveRoomInputs[0] = input_num;
                screen_num = 14;
            } else if (screen_num == 14) {
                reserveRoomInputs[1] = input_num;
                screen_num = 15;
            } else if (screen_num == 15) {
                reserveRoomInputs[2] = input_num;
                screen_num = 16;
            } else if (screen_num == 16) {
                reserveRoomInputs[3] = input_num;
                screen_num = 17;
            } else if (screen_num == 17) {
                reserveRoomInputs[4] = input_num;
                screen_num = 18;
            }

            else if(screen_num == 23)
            {
                editReservationInputs[0] = input_num;
                screen_num = 24;
            }
            else if(screen_num == 24)
            {
                editReservationInputs[1] = input_num;
                screen_num = 25;
            }
            else if(screen_num == 25)
            {
                editReservationInputs[2] = input_num;
                screen_num = 26;
            }
            else if(screen_num == 26)
            {
                editReservationInputs[3] = input_num;
                screen_num = 27;
            }
            else if(screen_num == 27)
            {
                editReservationInputs[4] = input_num;
                screen_num = 28;
            }
            else if(screen_num == 28)
            {
                editReservationInputs[5] = input_num;
                screen_num = 29;
            }
        }

    } 
   // screen logic how
            //after done, write out the screen num to see if it is actually changing ^^ispeek

int main(int argc, char **argv) {
    react = (argc > 1);
    read_text_data();

    bool just_starting = react ? _read_event_info() : true;
    while (true) {
        ifstream fs1(just_starting ? "initial_state" : "curr_state");
        State state(fs1);
        fs1.close();

        string input;
        if (just_starting) just_starting = false;
        else if (!react) getline(cin, input);

        ifstream fs2("incoming_text");
        state.update(input, fs2);
        ofstream fs3("outgoing_text"), fs4(react ? "end_state" : "curr_state");
        display(state, react ? fs3 : cout);
        state.write_to(fs4);

        if (react) {
            _write_react_yaml();
            break;
        }
    }

    return 0;
}
