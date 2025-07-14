// I got the general idea from here: (#8)
// https://hackr.io/blog/cpp-projects
// although I tried to consult it as little as possible.

// For input/output.
#include <iostream>
// For 'vector' data structure.
#include <vector>
// For files.
#include <fstream>
// For input stream limit.
#include <limits>
// For remove_if.
#include <algorithm>

using namespace std;

struct Contact {
    string name;
    string phone;
    string email;
};

// For each contact,
// prints its values.
void displayContacts( vector<Contact> contacts ) {
    cout << "\n";
    for ( struct Contact contact : contacts ) {
        cout << "Name:  " << contact.name + "\n";
        cout << "Phone: " << contact.phone + "\n";
        cout << "Email: " << contact.email + "\n";
        cout << "\n";
    }
}

// Asks user to input the name of a contact.
// If that contact exists,
// prints that contact's information.
void searchContact( vector<Contact> contacts ) {

    // Clears input stream.
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    // Gets name of Contact from user.
    string targetName;
    cout << "Name of contact: ";
    getline(cin, targetName);

    // Searches for the desired Contact.
    cout << "\n";
    for ( struct Contact contact : contacts ) {
        if ( contact.name == targetName ) {
            // Found it.
            cout << "Name:  " << contact.name + "\n";
            cout << "Phone: " << contact.phone + "\n";
            cout << "Email: " << contact.email + "\n";
            cout << "\n";
            return;
        }
    }

    cout << "Contact not found." << "\n\n";

}

// Creates a new contact.
void addContact( vector<Contact> &contacts ) {

    Contact newContact;

    // Clears input stream.
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    // Gets name, phone, and email from user.

    cout << "Input contact name:  " << "\n";
    getline(cin, newContact.name);

    cout << "Input contact phone: " << "\n";
    getline(cin, newContact.phone);
    
    cout << "Input contact email: " << "\n";
    getline(cin, newContact.email);

    cout << "\n";

    if ( newContact.name.find('|') != string::npos
        || newContact.phone.find('|') != string::npos
        || newContact.email.find('|') != string::npos ) {
        // Does not add the contact if it contains '|' anywhere
        // (as that is the delimiter).
        cout << "Error: cannot use character \'|\'." << "\n\n";
        return;
    }
    cout << "New contact added." << "\n\n";

    // Adds new Contact to contacts vector.
    contacts.push_back(newContact);
    
    // Opens file.
    ofstream outfile;
    outfile.open("Contacts.txt", ios_base::app);

    // Writes new Contact to file.
    outfile << "\n" << newContact.name;
    outfile << "|" << newContact.phone;
    outfile << "|" << newContact.email;

    outfile.close();

}

// Requests the name of a contact.
// If it exists, deletes that contact.
void deleteContact( vector<Contact> &contacts ) {

    // Clears input stream.
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    // Gets name of Contact from user.
    string targetName;
    cout << "Contact to delete: ";
    getline(cin, targetName);

    auto iterator = remove_if( contacts.begin(), contacts.end(), [&](const Contact &contact) {
        return contact.name == targetName; }
    );

    // The iterator points to the last non-removed element.
    // If nothing is removed, this will point to the end of the vector.
    // But if something is removed, it will point elsewhere.
    if ( iterator != contacts.end() ) {
        cout << "Contact removed." << "\n\n";

        // Removes the Contact from the vector.
        contacts.erase(iterator, contacts.end());

        // Also removes the contact from the file.

        // Gets Contacts from text file.
        ifstream instream("Contacts.txt");

        // Creates an empty file to eventually replace Contacts.txt.
        ofstream outstream;
        outstream.open("temp.txt", ofstream::out);

        string line;
        bool addEnter = false;
        while ( getline(instream, line) ) {
            // Keeps the line in the file,
            // unless the line's contact name matches the target.
            if ( line.substr(0, line.find_first_of('|') ) != targetName ) {
                // Ensures that there isn't an empty line at the end.
                if ( addEnter ) {
                    outstream << "\n";
                } else {
                    addEnter = true;
                }
                outstream << line;
            }
        }

        instream.close();
        outstream.close();
        
        // Deletes the original file,
        // replacing it with the updated one.
        remove("Contacts.txt");
        rename("temp.txt", "Contacts.txt");

    } else {
        // Not found.
        cout << "Contact not found." << "\n\n";
    }

}

int main() {

    // An empty vector of Contacts.
    vector<Contact> contacts;

    // Gets contacts from text file.
    ifstream MyReadFile("Contacts.txt");

    // Within the file,
    // each line will delimit a Contact.
    // Within a line,
    // the different fields will be delimited with a "|" symbol
    // (which also means that symbol should not be allowed).
    string line;
    while ( getline (MyReadFile, line) ) {

        // Sorts the line into 3 sections.
        string name = line.substr( 0, line.find_first_of('|') );
        line = line.substr(line.find_first_of('|') + 1, line.length() );
        string phone = line.substr( 0, line.find_first_of('|') );
        line = line.substr(line.find_first_of('|') + 1, line.length() );
        string email = line.substr( 0, line.length() );

        // Creates new Contact.
        Contact newContact = {name, phone, email};
        // Adds it to contacts vector.
        contacts.push_back(newContact);

    }

    MyReadFile.close();

    int input;
    // Runs until the user exits.
    do {

        cout << "1. Display Contacts" << "\n";
        cout << "2. Search Contact" << "\n";
        cout << "3. Add Contact" << "\n";
        cout << "4. Delete Contact" << "\n";
        cout << "5. Exit" << "\n";
        cout << "INPUT: ";

        // Gets input from user.
        cin >> input;

        if ( input < 1 || input > 5 ) {
            // Clears the input so that an invalid input
            // (like a char) doesn't destroy the whole thing.
            cin.clear();
            cin.ignore();
        }

        // Performs action based on input.
        switch ( input ) {
            case 1:
                displayContacts(contacts);
                break;
            case 2:
                searchContact(contacts);
                break;
            case 3:
                addContact(contacts);
                break;
            case 4:
                deleteContact(contacts);
                break;
            case 5:
                break;
            default:
                cout << "Invalid input." << "\n";
        }

    } while ( input != 5 );

    return 0;

}