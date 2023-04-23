#include <dataClass.h>

DataClass::DataClass()
{
    mID = 0;
    mName = "";
    mCity = "";
    mAddress = "";
    mBuildingDate = "";
    mCapacity = 0;
    mWorkingHours = "";
    mWebsiteAddress = "";
}
DataClass::DataClass(const int id,
                   const QString name,
                   const QString city,
                   const QString address,
                   const QString buildingDate,
                   const int capacity,
                   const QString workingHours,
                   const QString websiteAddress
                   )
{
    mID = id;
    mName = name;
    mCity = city;
    mAddress = address;
    mBuildingDate = buildingDate;
    mCapacity = capacity;
    mWorkingHours = workingHours;
    mWebsiteAddress = websiteAddress;
}

void DataClass::setID(const int id)
{
    mID = id;
}

void DataClass::setName(const QString name)
{
    mName = name;
}

void DataClass::setCity(const QString city)
{
    mCity = city;
}

void DataClass::setAddress(const QString address)
{
    mAddress = address;
}

void DataClass::setBuildingDate(const QString buildingDate)
{
    mBuildingDate = buildingDate;
}

void DataClass::setCapacity(const int capacity)
{
    mCapacity = capacity;
}

void DataClass::setWorkingHours(const QString workingHours) {
    mWorkingHours = workingHours;
}

void DataClass::setWebsiteAddress(const QString websiteAddress)
{
    mWebsiteAddress = websiteAddress;
}

int DataClass::getID()
{
    return mID;
}

QString DataClass::getName() const
{
    return mName;
}

QString DataClass::getCity() const
{
    return mCity;
}

QString DataClass::getAddress() const
{
    return mAddress;
}

QString DataClass::getBuildingDate() const
{
    return mBuildingDate;
}

int DataClass::getCapacity() const
{
    return mCapacity;
}

QString DataClass::getWorkingHours() const
{
    return mWorkingHours;
}

QString DataClass::getWebsiteAddress() const
{
    return mWebsiteAddress;
}



