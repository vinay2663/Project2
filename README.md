# Project2
Smart Parking system project using node mcu and RFID Tags

Project is implemented using nodeMCU and RFID tags

All the vehicle should contain RFID tags and the details of the owner of vehicles are stored in firebase database 

When RFID tag is read by nodeMCU the 12 digit unique tag is compared with tag value in database and owner's details are fetched 
then vehicles parked are uploaded in other table 

an android app is created to view the vehicles parked and an sms is sent to owner number mentioning about parking 
