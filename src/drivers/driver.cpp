#include <drivers/driver.h>
#include <common/types.h>

using namespace smhos::common;
using namespace smhos::drivers;

void printf(char*);
Driver::Driver() {

}

Driver::~Driver() {

}

void Driver::Activate() {
    printf("ACTIVATED\n");

}

int Driver::Reset() {
    return 0;
}

void Driver::Deactivate() {

}

DriverManager::DriverManager() {
    numDrivers = 0;
}

void DriverManager::AddDriver(Driver* drv){
    drivers[numDrivers] = drv;
    numDrivers++;
}

void DriverManager::ActivateAll() {
    for(int i = 0; i < numDrivers; i++){
        drivers[i]->Activate();
    }
}