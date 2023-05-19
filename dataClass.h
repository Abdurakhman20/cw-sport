#ifndef DATACLASS_H
#define DATACLASS_H

#include <QString>
#include <QVariant>
 // Класс с данными из файла
class DataClass {
private:
    int mID;
    QString mName;
    QString mCity;
    QString mAddress;
    QString mBuildingDate;
    int mCapacity;
    QString mWorkingHours;
    QString mWebsiteAddress;
public:
    DataClass();
    DataClass(const int mID,
            const QString mName,
            const QString mCity,
            const QString mAddress,
            const QString mBuildingDate,
            const int mCapacity,
            const QString mWorkingHours,
            const QString mWebsiteAddress
            );
    void setID(const int id);
    void setName(const QString name);
    void setCity(const QString city);
    void setAddress(const QString address);
    void setBuildingDate(const QString buildingDate);
    void setCapacity(const int capacity);
    void setWorkingHours(const QString workingHours);
    void setWebsiteAddress(const QString websiteAddress);

    int getID();
    QString getName() const;
    QString getCity() const;
    QString getAddress() const;
    QString getBuildingDate() const;
    int getCapacity() const;
    QString getWorkingHours() const;
    QString getWebsiteAddress() const;

};
Q_DECLARE_METATYPE(DataClass);

#endif // DATACLASS_H
