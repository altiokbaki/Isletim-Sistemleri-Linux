///İşletim Sistemleri Laboratuvarı 5. Uygulama
///Tarih: 21.05.2021
///Osman Baki ALTIOK/152120151055
///osmanbakialtiok@gmail.com
#include <unistd.h>                                                             //Linux-C interactions.
#include <string>                                                               //String operations.
#include <stdio.h>                                                              //C basic library.
#include <semaphore.h>                                                          //Semaphore operations.
#include <sys/wait.h>                                                           //Sleep,wait etc.
#include <stdlib.h>                                                             //C basic library.
#include <pthread.h>                                                            //Thread operations.
#include <iostream>                                                             //C++ basic library.
#define SIZE 100
using namespace std;                                                            //C++ things.
						
const int constant_value = 13;                                                  //Number of Products.
int counter = constant_value;                                                   //Counter for criticalRegion function.
int counter1=1;
pthread_mutex_t Mutex_Count;                                                    //Mutex_Lock.
pthread_mutex_t Mutex_Print;                                                    //Mutex_Print(Needed for indent but not sure if it helped).
/// <summary>
/// Constant char array of products.Should be changed to reuse(Works with print function).
/// </summary>
const char a[SIZE][SIZE] = { "Çilek","Muz","Kivi","Çamaşır suyu","Deterjan","Köfte","Tavuk","Balık","Mercimek","Pirinç","Baklava",
"Kadayıf","Künefe" };

/// <summary>
/// Semaphores Declaration.Should be changed to reuse(Works with Semaphor_Init function).
/// </summary>
sem_t sem0;                                                                     //Case 1->Çilek alındıktan sonra çamaşır suyu alınabilir.
sem_t sem1;                                                                     //Case 2->Mercimek alındıktan sonra kadayıf alınabilir.
sem_t sem2;                                                                     //Case 3->Tüm temizlik ürünleri alındıktan sonra balık alınabilir.
sem_t sem3;                                                                     //Case 4->Kadayıf alındıktan sonra et ürünleri alınmaya başlanabilir.
sem_t sem4;                                                                     //Case 5->En son kivi alınmalıdır.
/// <summary>
/// Mutex_Lock function.Returns int counter(Works with print function).
/// Decrements the value of counter by 1.
/// </summary>
int criticalRegion() {
    pthread_mutex_lock(&Mutex_Count);                                           //Lock critical region to make sure only 1 semaphor has reach critical section.
    if (counter >= 0) 
    {
        counter--;                                                              //Decrement counter by 1(Works with print function).
        counter1++;                                                             //Increment counter1 by 1(Works with print function).->Listing number.
    }
    else 
    {                                                                           //Error handling.
        counter = 0;
        cout << "ERROR! Overflow on counter value." << endl;    
    }

    if (counter == 1) {
        sem_post(&sem4);                                                        //!En son kivi alınmalıdır(Tüm ürünler alındı).
    }

    pthread_mutex_unlock(&Mutex_Count);                                         //Unlock critical section(Semaphore has done with its queue).
    return counter;
}
/// <summary>
/// Takes an integer value and string.Prints the value of the array in given index(Formatted).
/// </summary>
void print(int b, string s) {
	
    if (b <= constant_value) {
        pthread_mutex_lock(&Mutex_Print);
        cout << counter1 << ".\t" << s << "\tUrun:" << "-->" << a[b] << "\tKalan urun sayisi\t" << criticalRegion() << endl;
        pthread_mutex_unlock(&Mutex_Print);
    }
    else
    {
        cout << "There is no element in given position." << endl;               //Error handling.
    }
}
/// <summary>
/// Used by thread_Meyve thread.
/// </summary>
void* WMeyve(void* arg)
{
    print(0, "(Meyve)");                                                        //"Çilek"
    sem_post(&sem0);                                                            //!Çilek alındıktan sonra çamaşır suyu alınabilir(Çilek alındı).
    print(1, "(Meyve)");                                                        //"Muz"
    sem_wait(&sem4);                                                            //!En son kivi alınmalıdır(Tüm ürünlerin alınmasını bekle).
    print(2, "(Meyve)");                                                        //"Kivi"
    cout << "-->\t\t\tMeyve kategorisindeki tum urunler alindi." << endl;       //End of category "Meyve".
}
/// <summary>
/// Used by thread_Temizlik thread.
/// </summary>
void* WTemizlik(void* arg)
{
    sem_wait(&sem0);                                                            //!Çilek alındıktan sonra çamaşır suyu alınabilir(Çileğin alınmasını bekle).
    print(3, "(Temizlik)");                                                     //"Çamaşır suyu"
    print(4, "(Temizlik)");                                                     //"Deterjan"
    sem_post(&sem2);                                                            //!Tüm temizlik ürünleri alındıktan sonra balık alınabilir(Tüm temizlik ürünleri alındı).
    cout << "-->\t\t\tTemizlik kategorisindeki tum urunler alindi." << endl;    //End of category "Temizlik".  
}
/// <summary>
/// Used by thread_Et thread.
/// </summary>
void* WEt(void* arg)
{   
    sem_wait(&sem3);                                                            //!Kadayıf alındıktan sonra et ürünleri alınmaya başlanabilir(Kadayıfın alınmasını bekle).
    print(5, "(Et)");                                                           //"Köfte"
    print(6, "(Et)");                                                           //"Tavuk"
    sem_wait(&sem2);                                                            //!Tüm temizlik ürünleri alındıktan sonra balık alınabilir(Tüm temizlik ürünlerinin alınmasını bekle).
    print(7, "(Et)");                                                           //"Balık"
    cout << "-->\t\t\tEt kategorisindeki tum urunler alindi." << endl;          //End of category "Et".
}
/// <summary>
/// Used by thread_Bakliyat thread.
/// </summary>
void* WBakliyat(void* arg)
{
    print(8, "(Bakliyat)");                                                     //"Mercimek"
    sem_post(&sem1);                                                            //!Mercimek alındıktan sonra kadayıf alınabilir(Mercimek alındı).
    print(9, "(Bakliyat)");                                                     //"Pirinç"
    cout << "-->\t\t\tBakliyat kategorisindeki tum urunler alindi." << endl;    //End of category "Bakliyat".
}
/// <summary>
/// Used by thread_Tatli thread.
/// </summary>
void* WTatli(void* arg)
{    
    print(10, "(Tatli)");                                                       //"Baklava"
    sem_wait(&sem1);                                                            //!Mercimek alındıktan sonra kadayıf alınabilir(Mercimeğin alınmasını bekle).
    print(11, "(Tatli)");                                                       //"Kadayıf"
    sem_post(&sem3);                                                            //!Kadayıf alındıktan sonra et ürünleri alınmaya başlanabilir(Kadayıf alındı).
    print(12, "(Tatli)");                                                       //"Künefe"
    cout << "-->\t\t\tTatli kategorisindeki tum urunler alindi." << endl;       //End of category "Tatli".
}
/// <summary>
/// Semaphor Initialization function.Generates semaphores with error handling.
/// </summary>
void Semaphor_Init(sem_t sem) 
{
    int control;                                                                //Control value for error handling.
    control = sem_init(&sem, 0, 0);                                             //sem_init function returns 0 if success.
    if (control != 0)                                                           //Else its a perror.
    {
        perror("Failed to initialize semaphore sem");
    }
}
/// <summary>
/// Semaphor Destroying function.Destroys semaphores with error handling.
/// </summary>
void Semaphor_Destroy(sem_t sem) {
   									                                            //Semaphores should be destroyed after use.
    int control;                                                                //Control value for error handling.
    control = sem_destroy(&sem);                                	            //sem_destroy function returns 0 if success.
    if (control != 0)                                                           //Else is a perror.
    {
        perror("Failed to destroy semaphore sem");
	}
}
/// <summary>
/// Main function.Takes -lpthread as argument.
/// </summary>
int main(int argc, char* argv[])
{
   	//Start of the list.
    cout << "\tProgram Starts Here!" << endl;
    pthread_t thread_Meyve, thread_Temizlik, thread_Et, thread_Tatli, thread_Bakliyat;
    //Created 5 Threads.
    pthread_create(&thread_Meyve, NULL, WMeyve, NULL);
    pthread_create(&thread_Temizlik, NULL, WTemizlik, NULL);
    pthread_create(&thread_Tatli, NULL, WTatli, NULL);
    pthread_create(&thread_Bakliyat, NULL, WBakliyat, NULL);
    pthread_create(&thread_Et, NULL, WEt, NULL);
 
    //Initialized 5 Semaphores.
    Semaphor_Init(sem0);
    Semaphor_Init(sem1);
    Semaphor_Init(sem2);
    Semaphor_Init(sem3);
    Semaphor_Init(sem4);
    
    //Joined 5 Threads.
    pthread_join(thread_Meyve, NULL);
    pthread_join(thread_Temizlik, NULL);
    pthread_join(thread_Tatli, NULL);
    pthread_join(thread_Bakliyat, NULL);
    pthread_join(thread_Et, NULL);
 
    //Destroyed 5 Semaphores.
    Semaphor_Destroy(sem0);
    Semaphor_Destroy(sem1);
    Semaphor_Destroy(sem2);
    Semaphor_Destroy(sem3);
    Semaphor_Destroy(sem4);
 
    return 0;
}//End of main.
