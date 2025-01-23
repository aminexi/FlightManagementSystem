# Flight Management System

## Overview
The flight Management System is a software solution designed to manage trips, clients, and bookings for a fictional travel agency. Developed using object-oriented programming principles, it allows for the efficient management of trips, client reservations, and additional services like excursions and packages.

## Features
- **View Available Trips**: Display all available trips with detailed information.
- **Book a Trip**: Reserve a trip with a choice of transport (Plane, Train).
- **Cancel Booking**: Cancel a previously made reservation.
- **Search Trips by Destination**: Find trips by entering the destination.
- **Add a Package**: Include multiple trips in a single package.
- **Client Details**: Display detailed information about registered clients.

## Design
The system is based on several core entities such as:
- **Destination**: Travel destination details (name, description, cost).
- **Transport**: Abstract base class for transport modes (Plane, Train).
- **Trip**: Specific trip details (destination, price, transport).
- **Client**: Represents a travel agency client.
- **TravelAgency**: Manages trips, clients, and reservations.

## Technologies Used
- **Language**: C++
- **Programming Paradigm**: Object-Oriented Programming (OOP)
- **Principles**: Inheritance, Polymorphism, Encapsulation
