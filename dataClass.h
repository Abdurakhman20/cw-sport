#ifndef DATACLASS_H
#define DATACLASS_H

#include <QString>
#include <QVariant>

class Supplier {
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
    Supplier();
    Supplier(
            const int mID,
            const QString mName,
            const QString mCity,
            const QString mAddress,
            const QString mBuildingDate,
            const int mCapacity,
            const QString mWorkingHours,
            const QString mWebsiteAddress,
            );
    void setId(const int id);
    void setName(const QString name);
    void setCity(const QString city);
    void setAddress(const QString address);
    void setBuildingDate(const QString mBuildingDate);
    void setCapacity(const int mCapacity);
    void setWorkingHours(const QString mWorkingHours);
    void setWebsiteAddress(const QString mWebsiteAddress);

    int getId();
    QString getName() const;
    QString getCity() const;
    QString getAdress() const;
    QString getBuildingDate() const;
    int getCapacity();
    QString getWorkingHours() const;
    QString getWebsiteAdress() const;

};
Q_DECLARE_METATYPE(Supplier);

#endif // DATACLASS_H
