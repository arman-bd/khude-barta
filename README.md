# Khude Barta (খুদে বার্তা)
Khude Barta is an online chatting application. Primary motive of this project is to display the possibility of integrating internet enabled service in desktop application. There are many different possibility of using this technology in real life.

## Main Features
* User Authentication
* Instant Messaging 
* Group Chatting

## Technologies Used
* C – Programming Language
* Apache – Web Server
* PHP – Server Side Scripting Language
* MySQL – Database Program
* Electron – Cross Platform Application Builder
* HTML / CSS / JavaScript – Web Development Technologies
 
## Network Diagram

<a href="#"><img src="https://i.imgur.com/Ufdtuxi.png" width="450"/></a>

## Database Structure
We used MySQL as our database backend. Our web service stores and retrieve data from here. To have minimum functionality in our project we created two table on a database “user” & “message”. One is for storing & verifying user information and other is used to store & retrieve messages for users.

### User Table

<a href="#"><img src="https://i.imgur.com/89Ghdaw.png" width="650"/></a>

### Message Table

<a href="#"><img src="https://i.imgur.com/DQqhFy6.png" width="650"></a>

## Web Service 
Backbone of this project is web service behind it. Our web service is built using PHP / MySQL. Our web service runs on Apace Web Server. Main parts of our service are Authentication, Send, Receive & Transceiver. 
* <b>Authentication</b> – When a client tries to connect to service it verifies the authenticity of the user by matching username & password stored in database. After verifying this service returns an access token to the user.
* <b>Send</b> – When an authenticated user sends a message this endpoint receives it & store it in database.
* <b>Receive</b> – When an authenticated client contacts this endpoint, it returns new messages in database.
* <b>Transceiver</b> – This endpoint is for sending & receiving at the same time. Our C based client uses this endpoint for faster access.

## C Based Client
Most important part of this project is to display the possibility of creating a fully functional C based chatting client.

<a href="#"><img src="http://i.imgur.com/znMIuYW.png" width="650"/></a>

Our C client uses Socket & Thread. Socket helps in accessing internet; Thread helps in sending & receiving message at the same time without blocking the standard input / output stream. The main benefit of this C client is it can run on different platforms.

## Desktop Client
Our desktop client is built using <a href="https://github.com/electron/electron">Electron</a> which is a cross platform application builder. Due to this our application can run on most of the popular operating systems out there.

<a href="#"><img src="https://i.imgur.com/AWOTExi.png" width="650"/></a>

## Web Client
Our web client is built using HTML / CSS / JavaScript and it uses the web service for authenticating, sending & receiving message.

<a href="#"><img src="https://i.imgur.com/lUS13tp.png" width="650"></a>

## Mobile Client
Interface for our mobile client looks symmetrical to our desktop & web interface. Since our theme is responsive we can fit it into almost any size of display.

<a href="#"><img src="https://i.imgur.com/5AF82OM.jpg" width="650"/></a>


## This Project is Done By:

* <a href="https://github.com/arman-bd">Arman Hossain</a>
* <a href="https://github.com/tarinfatema">Tarin Fatema Mithila</a>
* Shahinur Shamshad
* <a href="https://github.com/Maaaaruf">Mariful Islam</a>

CSE, 11'th Batch,<br>
Shanto-Mariam University of Creative Technology,<br>
Dhaka, Bangladesh.<br>