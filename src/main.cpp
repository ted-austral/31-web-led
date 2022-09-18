/*
 *  Project 31-web-led - main.cpp
 *      LED managment through a Web page
 *
 *      This Web page, resident in the same ESP32 allows to change
 *      the LED state.
 *
 *      It is a case of stateless control
 *
 *      Austral - IntroComp - 2022 - EAM
 */

#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>

#include "wifi_ruts.h"

WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80 

void
handleRoot( void )
{
// When URI / is requested, send a web page with a button to toggle the LED
    Serial.println( __FUNCTION__ );

    server.send
    (
        200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>"
    );
}

void
handleLED( void )
{
// If a POST request is made to URI /LED, then toggle LED state
    Serial.println( __FUNCTION__ );

    digitalWrite( LED, !digitalRead( LED ) );   // Change the state of the LED
    server.sendHeader( "Location", "/" );       // Add a header to respond with a new location for the browser to go to the home page again
    server.send(303);                           // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void
handleNotFound( void )
{
    Serial.println( __FUNCTION__ );

    server.send(404, "text/plain",
                "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void
setup(void)
{
    Serial.begin(SERIAL_BAUD); // Start the Serial communication to send messages to the computer
    delay(10);
    Serial.println();

    pinMode(LED, OUTPUT);

    wifi_connect();

    server.on( "/",     HTTP_GET,   handleRoot);
    server.on( "/LED",  HTTP_POST,  handleLED);
    server.onNotFound( handleNotFound);         // When a client requests an unknown

    server.begin();     // Actually start the server
    Serial.println("HTTP server started");
}

void
loop(void)
{
    server.handleClient(); // Listen for HTTP requests from clients
}
