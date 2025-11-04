#include <iostream>
#include <fstream>
#include <regex>
#include <windows.h>
#define NOMINMAX
#include <string>
#include <limits>
#include <iomanip>

using namespace std;

class Patient
{
    private:
        int ID;
        int age;
        string surname_patient;
        string gender;
        string surname_doctor;
        string diagnoz;
        double temperture;
        int ward;
    public:
            //с параметрами конструктор
            Patient(int i, int a, string s_patient, string g, string s_doctor, string d, double t, int w){
                ID = i;
                age = a;
                surname_patient = s_patient;
                gender = g;
                surname_doctor = s_doctor;
                diagnoz = d;
                temperture = t;
                ward = w;
            }
            //по умолчанию конструктор
            Patient(){
                ID = 0;
                age = 0;
                surname_patient = "NO SURNAME";
                gender = "mf";
                surname_doctor = "NO SURNAME";
                diagnoz = "NO DIAGNOZ";
                temperture = 36.6;
                ward = 0;
            }

            ~Patient() {} //деконструктор


            //геттер и сеттер для айди пациента
            int getID(){return ID;};
            void setID(int i){ID = i;}
            
            //геттер и сеттер для фамилии пациента
            string getSurnamePatient(){return surname_patient;};
            void setSurnamePatient(string s){surname_patient=s;}

            //геттер и сеттер для фамилия доктора
            string getSurnameDoctor(){return surname_doctor;};
            void setSurnameDoctor(string s){surname_doctor = s;};

            //геттер и сеттер для пола
            string getGender(){return gender;};
            void setGender(string g){gender = g;};

            //геттер и сеттер для возраста
            int getAge(){return age;};
            void setAge(int a){age = a;};

            //геттер и сеттер для диагноза
            string getDiagnoz(){return diagnoz;};
            void setDiagnoz(string d){diagnoz = d;};

            //геттер и сеттер для температуры
            double getTemperture(){return temperture;};
            void setTemperture(double t){temperture = t;};

            //геттер и сеттер для палаты
            int getWard(){return ward;};
            void setWard(int w){ward= w;};
            
};


Patient* Patients = nullptr;
int PatientsCount =0;
int capacity = 0;

// Функция для очистки буфера ввода
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


void show_massive(){
    if (Patients == nullptr || PatientsCount == 0) {
        cout << "No patients in database!" << endl;
        return;
    };
    // Шапка таблицы
    cout << "\n";
    cout << "==========================================================================================================" << endl;
    cout << "| ID  | Фамилия       | Пол | Возраст | Диагноз   | Температура | Лечащий врач        | Палата |" << endl;
    cout << "==========================================================================================================" << endl;
    
    // Данные пациентов
    for (int i = 0; i < PatientsCount; i++) {
        cout << "| " << setw(2) << Patients[i].getID()<< " "
             << "| " << setw(13) << left << Patients[i].getSurnamePatient() << " "
             << "| " << setw(6) << left << Patients[i].getGender() << " "
             << "| " << setw(3) << right << Patients[i].getAge() << " "
             << "| " << setw(18) << left << Patients[i].getDiagnoz() << " "
             << "| " << setw(11) << right << fixed << setprecision(1) << Patients[i].getTemperture() << " "
             << "| " << setw(13) << left << Patients[i].getSurnameDoctor() << " "
             << "| " << setw(4) << right << Patients[i].getWard() << " |" << endl;
    }
    
    cout << "==========================================================================================================" << endl;
    cout << "Total patients: " << PatientsCount << endl;
};

// Проверка, что строка соответсвует шаблону
bool isValidName(const string& str) {
    if (str.empty()) return false;
    regex pattern(R"([A-Za-zА-Яа-я]+_[A-Za-zА-Яа-я]\.[A-Za-zА-Яа-я]\.)");
    if (regex_match(str, pattern)) return true;
    else{return false;};
}
//проверка диагноза
bool isValidDiagnoz(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        // Запрещаем только цифры
        if (!isalpha(c) && c != ' ' && c != '-' && c != '.' && c != ',') {
            return false;
        }
    }
    return true;
}

// Проверка возраста (от 0 до 150 лет)
bool isValidAge(int age) {
    return age >= 0 && age <= 150;
}

// Проверка номера палаты (положительное число)
bool isValidRoom(int room) {
    return room > 0;
}

// Проверка температуры (разумный диапазон от 30 до 45 градусов)
bool isValidTemperature(double temp) {
    return temp >= 30.0 && temp <= 45.0;
}

//чтение файла
Patient* read_file(int& PatientsCount, int& capacity){
    //читаем файл
    ifstream file("Patients.txt");
    if (!file.is_open()){
        cout<<"Error!";
        return nullptr;
    }
    //считаем кол-во строк
    int count = 0;
    string line;
    while (getline(file,line)){
        if (!line.empty())
            count++;
    };

    if (count == 0){
        cout<<"Error!";
        file.close();
        return nullptr;
    }

    file.clear();
    file.seekg(0);


    PatientsCount = count;
    capacity = count + 10;
    Patient* Patients = new Patient[capacity];


    for (int i=0; i<PatientsCount;i++){
        int id;
        int age;
        string s_patient, gender, s_doctor, diagnoz;
        double temp;
        int ward;
        if (file>>id>>s_patient>>gender>>age>>diagnoz>>temp>>s_doctor>>ward){
            Patients[i]=Patient(id, age, s_patient, gender, s_doctor, diagnoz, temp, ward);
        }else{
            cout<<"error reading!"<<endl;
            break;
        }
    };

    file.close();
    return Patients;
};

void increase_massive(){
    int new_capacity = capacity*2;
    Patient* new_patients = new Patient[new_capacity];

    for (int i=0; i<PatientsCount;i++ ){
        new_patients[i] = Patients[i];
    }

    delete[] Patients;

    Patients = new_patients;
    capacity = new_capacity;
}

void delete_patients(){
    show_massive();
    if (PatientsCount==0){
        cout<<"No patients in base!!!"<<endl;
        return;
    }
    int id;
    cout<<"Введите ID пациента, которого хотите удалить: "; cin>>id;

    bool found = false;

    for (int i=0; i<PatientsCount; i++){
        if (id==Patients[i].getID()){
            found = true;
            for (int j = i; j<PatientsCount - 1; j++){
                Patients[j] = Patients[j+1];
                Patients[j].setID(j+1);
            }
            PatientsCount--;
            cout<<"Patient deleted!!!"<<endl;
        }
    }
    if (found == false){
            cout<<"Patient is not found!"<<endl;
    }
}


void add_patient(){
    if (PatientsCount>=capacity){
        increase_massive();
    }

    Patient new_patient;

    new_patient.setID(PatientsCount+1);

    while (true){
        string surname;
        cout<<"Введите фамилию пациента (Фамилия_И.О.): ";cin>>surname;
        if (isValidName(surname)){
            new_patient.setSurnamePatient(surname);
            break;
        }else{
            cout<<"Ошибка! Введите имя по шаблону"<<endl;
            clearInputBuffer();
        }
    }
    while (true){
        cout<<"Введите пол M/F ";
        string gender;cin>>gender;
        if (gender == "M" || gender == "m" || gender == "F" || gender == "f"){
            new_patient.setGender(gender);
            break;
        }else{
            cout<<"Гендера только два"<<endl;
            clearInputBuffer();
        }
    }

    while(true){
        cout<<"Введите возраст пациента: ";
        int age;cin>>age;
        if (isValidAge(age)){
            new_patient.setAge(age);
            break;
        }else{
            cout<<"Некорректный возраст!"<<endl;
            clearInputBuffer();
        }
    }

    while (true){
        cout<<"Введите диагноз пациента: ";
        string diagnoz;cin>>diagnoz;
        if (isValidDiagnoz(diagnoz)){
            new_patient.setDiagnoz(diagnoz);
            break;
        }else{
            cout<<"Некорректный диагноз";
            clearInputBuffer();
        }
    }

    while(true){
        cout<<"Введите температуру пациента: ";
        double temp;cin>>temp;
        if (isValidTemperature(temp)){
            new_patient.setTemperture(temp);
            break;
        }else{
            cout<<"Некорректная температура! Температура должна быть в диапазоне от 30.0 до 45.0 градусов."<<endl;
            clearInputBuffer();
        }
    }

    while (true){
        cout<<"Введите фамилию лечащего врача (Фамилия_И.О.): ";
        string doctor;cin>>doctor;
        if (isValidName(doctor)){
            new_patient.setSurnameDoctor(doctor);
            break;
        }else{
            cout<<"Некорректный шаблон!"<<endl;
            clearInputBuffer();
        }
    }

    while(true){
        cout<<"Введите номер палаты: ";
        int w;cin>>w;
        if (isValidRoom(w)){
            new_patient.setWard(w);
            break;
        }else{
            cout<<"Некорректный номер палаты!"<<endl;
            clearInputBuffer();
        }
    }

    clearInputBuffer();
    Patients[PatientsCount] = new_patient;
    PatientsCount++;

}


void change_patient(){
    show_massive();
    bool found = false;
    cout<<"Введите ID пациента для изменения: ";
    int id; cin>>id;
    for (int i= 0; i<PatientsCount; i++){
        if (id == Patients[i].getID()){
            found = true;
            clearInputBuffer();
            cout<<R"(
        Выберите что хотите изменить:
            1. Имя лечащего врача.
            2. Температуру пациента.
            3. Номер палаты пациента.
            4. Диагноз пациента)"<<endl;
            int choice;
            cin>>choice;
            while(true){
                if (cin.fail()){
                    cout<<"Некорректный ввод!";
                    clearInputBuffer();
                }else{
                    break;
                }
            }

            switch(choice){
                case 1:{
                while (true){
                    string new_doctor;
                    cout<<"Введите новое имя лечащего врача (Фамилия_И.О.): ";cin>>new_doctor;
                    if (isValidName(new_doctor)){
                        Patients[i].setSurnameDoctor(new_doctor);
                        cout<<"Данные врача обновлены!";
                        break;
                    }else{
                        cout<<"Некорректный ввод!";
                        clearInputBuffer();
                    }
                }
                break;}
                case 2:{
                while (true){
                    cout<<"Введите новую температуру: ";
                    double t;cin>>t;
                    if (isValidTemperature(t)){
                        Patients[i].setTemperture(t);
                        cout<<"Данные о температуре пациента обновлены!";
                        break;
                    } else{
                        cout<<"Некорректная температура!";
                        clearInputBuffer();
                    }
                }
                break;}
                case 3:{
                while (true){
                    int w;
                    cout<<"Введите номер новой палаты: "; cin>>w;
                    if (isValidRoom(w)){
                        Patients[i].setWard(w);
                        cout<<"Данные о номере палаты обновлены!";
                        break;
                    }else{
                        cout<<"Неверный ввод палаты!";
                        clearInputBuffer();
                    }
                }
                break;}
                case 4:{
                while(true){
                    string diag;
                    cout<<"Введите новый диагноз: "; cin>>diag;
                    if (isValidDiagnoz(diag)){
                        Patients[i].setDiagnoz(diag);
                        cout<<"Данные о диагнозе обновлены!";
                        break;
                    }else{
                        cout<<"Неверный ввод диагноза!";
                        clearInputBuffer();
                    }
                }
                break;}
                default:{
                cout<<"Неверный выбор!";}
            }
        }
    }
    if (found == false){
        cout<<"Пациент не найден!";
        return;
    }
}

void calculate(){
    //средняя температура
    //максимальный и минимальный возраст
    //средний возраст
    //колво пациентов

    int max = 1;
    int min = 151;
    int age_all = 0;

    for (int i=0; i<PatientsCount; i++){
        age_all += Patients[i].getAge();
        if (Patients[i].getAge() >= max) max = Patients[i].getAge();
        if (Patients[i].getAge() <= min) min = Patients[i].getAge();
    }

    cout<<"===СТАТИСТИКА==="<<endl;
    cout<<"Количество пациентов: "<<PatientsCount<<endl;
    cout<<"Средний возраст пациентов: "<< age_all / PatientsCount<<endl;
    cout<<"Максимальный возраст: "<< max<<endl;
    cout<<"Минимальный возраст: "<< min<<endl;


}

void save_to_file(){
    if (PatientsCount == 0) {
        cout << "Нет данных для сохранения!" << endl;
        return;
    }
    string filename;
    cout<<"Введите имя нового файла: "; cin>>filename;
    size_t dot = filename.find_last_of('.');
    if (dot != string::npos){
        filename = filename.substr(0,dot); //только имя без расширения
    }
    filename+=".txt";
    
    ofstream file(filename);
    if (!file.is_open()){
        cout<<"Ошибка создания файла!"<<endl;
        return;
    }

    for (int i=0; i<PatientsCount;i++){
         file << Patients[i].getID() << " "
             << Patients[i].getSurnamePatient() << " "
             << Patients[i].getGender() << " "
             << Patients[i].getAge() << " "
             << Patients[i].getDiagnoz() << " "
             << Patients[i].getTemperture() << " "
             << Patients[i].getSurnameDoctor() << " "
             << Patients[i].getWard();
        if (i < PatientsCount - 1) {
                file << endl;  // Не добавляем перенос после последней строки
            }
    }
    file.close();
    cout << "Данные успешно сохранены в файл: " << filename << endl;
}

int main(){
    system("chcp 65001 > nul");
    
    Patients = read_file(PatientsCount, capacity);
    
    if (Patients == nullptr) {
        cout << "Ошибка загрузки данных!" << endl;
        return 1;
    }
    
    bool flag = true;
    while(flag){
        cout << "  === МЕНЮ ===" << endl;
        cout << "1. Показать всех пациентов" << endl;
        cout << "2. Добавить пациента" << endl;
        cout << "3. Удалить пациента" << endl;
        cout << "4. Изменить данные пациента" << endl;
        cout << "5. Показать статистику" << endl;
        cout << "6. Сохранить и выйти" << endl;
        cout << "Выберите действие: ";
        
        int choice;
        cin >> choice;
        
        if (cin.fail()) {
            cout << "Некорректный ввод!" << endl;
            clearInputBuffer();
            continue;
        }
        
        switch(choice) {
            case 1:
                show_massive();
                break;
            case 2:
                add_patient();
                break;
            case 3:
                delete_patients();
                break;
            case 4:
                change_patient();
                break;
            case 5:
                calculate();
                break;
            case 6:{
                save_to_file();
                flag = false;
                break;
            }
            default:
                cout << "Неверный выбор!" << endl;
        }
    }
    
    // Освобождение памяти перед выходом
    if (Patients != nullptr) {
        delete[] Patients;
        Patients = nullptr;
    }
    return 0;
}