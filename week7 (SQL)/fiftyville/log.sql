-- Keep a log of any SQL queries you execute as you solve the mystery.

-- get crime scene description related with the duck thief
select description
from crime_scene_reports
where description like '%duck%';
-- RESULT
-- id=295
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
-- year 2023 month 7 day 28

-- trasncription of the 3 witnesses
-- select transcript
select transcript, month, day
from interviews
where transcript like '%bakery%' and month = 7;
-- RESULT
-- (1)  Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
--      If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.

-- (2)  I don't know the thief's name, but it was someone I recognized.
--      Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

-- (3)  As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
--      In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
--      The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- parking register in the aproximation time of the theft.
select hour, minute, license_plate, activity
from bakery_security_logs
where year = 2023 and month = 7 and day = 28 and hour >= 9 and hour <= 10;
-- RESULT
-- +------+--------+---------------+----------+
-- | hour | minute | license_plate | activity |
-- +------+--------+---------------+----------+
-- | 9    | 14     | 4328GD8       | entrance |
-- | 9    | 15     | 5P2BI95       | entrance |
-- | 9    | 20     | 6P58WS2       | entrance |
-- | 9    | 28     | G412CB7       | entrance |
-- | 10   | 8      | R3G7486       | entrance |
-- | 10   | 14     | 13FNH73       | entrance |
-- | 10   | 16     | 5P2BI95       | exit     |
-- | 10   | 18     | 94KL13X       | exit     |
-- | 10   | 18     | 6P58WS2       | exit     |
-- | 10   | 19     | 4328GD8       | exit     |
-- | 10   | 20     | G412CB7       | exit     |
-- | 10   | 21     | L93JTIZ       | exit     |
-- | 10   | 23     | 322W7JE       | exit     |
-- | 10   | 23     | 0NTHK55       | exit     |
-- | 10   | 35     | 1106N58       | exit     |
-- | 10   | 42     | NRYN856       | entrance |
-- | 10   | 44     | WD5M8I6       | entrance |
-- | 10   | 55     | V47T75I       | entrance |
-- +------+--------+---------------+----------+

-- MAIN SUSPECTS --> THEY EXIT THE PARKING BETWEEN THE THEFT HOUR, THEY WERE IN THE BAKN DOIND TRANSACTIONS, THEY DID A PHONE CALL LESS THAN 60 SECONDS
select people.name, people.license_plate, people.passport_number, phone_calls.caller, phone_calls.receiver, phone_calls.duration, bakery_security_logs.minute
from people
join phone_calls on people.phone_number = phone_calls.caller
join bakery_security_logs on people.license_plate = bakery_security_logs.license_plate
join bank_accounts on people.id = bank_accounts.person_id
join atm_transactions on bank_accounts.account_number = atm_transactions.account_number
where bakery_security_logs.year = 2023 and bakery_security_logs.month = 7 and bakery_security_logs.day = 28 and bakery_security_logs.hour >= 9 and bakery_security_logs.hour <= 10
and bakery_security_logs.activity = 'exit'
and atm_transactions.year = 2023 and atm_transactions.month = 7 and atm_transactions.day = 28 and atm_transactions.atm_location = 'Leggett Street'
and phone_calls.year = 2023 and phone_calls.month = 7 and phone_calls.day = 28 and phone_calls.duration <= 60;

-- RESULT
-- +--------+---------------+-----------------+----------------+----------------+----------+
-- |  name  | license_plate | passport_number |     caller     |    receiver    | duration |
-- +--------+---------------+-----------------+----------------+----------------+----------+
-- | Bruce  | 94KL13X       | 5773159633      | (367) 555-5533 | (375) 555-8161 | 45       |
-- | Diana  | 322W7JE       | 3592750733      | (770) 555-1861 | (725) 555-3243 | 49       |
-- | Taylor | 1106N58       | 1988161715      | (286) 555-6063 | (676) 555-6554 | 43       |
-- +--------+---------------+-----------------+----------------+----------------+----------+

-- FLIGHTS OF THE MAIN SUSPECTS (THIEFT WAS FLYING NEXT DAY EARLY IN THE MORNING)
select people.name, *
from passengers
join people on passengers.passport_number = people.passport_number
join flights on passengers.flight_id = flights.id
where (passengers.passport_number = 5773159633 or passengers.passport_number = 3592750733 or passengers.passport_number = 1988161715)
and flights.day = 29 and  flights.hour < 12;

-- RESULT
-- +--------+-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+----+-------------------+------------------------+------+-------+-----+------+--------+
-- |  name  | flight_id | passport_number | seat |   id   |  name  |  phone_number  | passport_number | license_plate | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +--------+-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+----+-------------------+------------------------+------+-------+-----+------+--------+
-- | Bruce  | 36        | 5773159633      | 4A   | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       | 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
-- | Taylor | 36        | 1988161715      | 6D   | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       | 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
-- +--------+-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+----+-------------------+------------------------+------+-------+-----+------+--------+

-- POSIBLE ACCOMPLICES (RECEIVER PHONES OF CALLS OF BRUCE AND TAYLOR)
select *
from people
where phone_number = '(375) 555-8161' or phone_number = '(676) 555-6554';
-- RESULT
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 250277 | James | (676) 555-6554 | 2438825627      | Q13SVG6       |
-- | 864400 | Robin | (375) 555-8161 | NULL            | 4V16VO0       |
-- +--------+-------+----------------+-----------------+---------------+

-- END
-- ---------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------

-- MORE COMMANDS AND TRIES

-- LOGS OF 1106N58 license_plate
select year, month, day, hour, minute, license_plate, activity
from bakery_security_logs
where license_plate = '1106N58';
-- RESULT:
-- +------+-------+-----+------+--------+---------------+----------+
-- | year | month | day | hour | minute | license_plate | activity |
-- +------+-------+-----+------+--------+---------------+----------+
-- | 2023 | 7     | 28  | 8    | 34     | 1106N58       | entrance |
-- | 2023 | 7     | 28  | 10   | 35     | 1106N58       | exit     |
-- +------+-------+-----+------+--------+---------------+----------+

-- find person of the detected licence_plate
select *
from people
where license_plate = '1106N58';
-- +--------+--------+----------------+-----------------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +--------+--------+----------------+-----------------+---------------+
-- | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- +--------+--------+----------------+-----------------+---------------+

-- ATM BANK logs in location and day
select *
from atm_transactions
where year = 2023 and month = 7 and day = 28 and atm_location = 'Leggett Street';


-- necesito people, bank_accounts, atm_transactions
select *
from people
join bank_accounts on people.id = bank_accounts.person_id
join atm_transactions on bank_accounts.account_number = atm_transactions.account_number
where atm_transactions.year = 2023 and atm_transactions.month = 7 and atm_transactions.day = 28 and atm_transactions.atm_location = 'Leggett Street';

-- RESULT
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+-----+----------------+------+-------+-----+----------------+------------------+--------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year | id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | 686048    | 2010          | 267 | 49610011       | 2023 | 7     | 28  | Leggett Street | withdraw         | 50     |
-- | 948985 | Kaelyn  | (098) 555-1164 | 8304650265      | I449449       | 86363979       | 948985    | 2010          | 275 | 86363979       | 2023 | 7     | 28  | Leggett Street | deposit          | 10     |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       | 26013199       | 514354    | 2012          | 336 | 26013199       | 2023 | 7     | 28  | Leggett Street | withdraw         | 35     |
-- | 458378 | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       | 16153065       | 458378    | 2012          | 269 | 16153065       | 2023 | 7     | 28  | Leggett Street | withdraw         | 80     |
-- | 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       | 28296815       | 395717    | 2014          | 264 | 28296815       | 2023 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       | 25506511       | 396669    | 2014          | 288 | 25506511       | 2023 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       | 28500762       | 467400    | 2014          | 246 | 28500762       | 2023 | 7     | 28  | Leggett Street | withdraw         | 48     |
-- | 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       | 76054385       | 449774    | 2015          | 266 | 76054385       | 2023 | 7     | 28  | Leggett Street | withdraw         | 60     |
-- | 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       | 81061156       | 438727    | 2018          | 313 | 81061156       | 2023 | 7     | 28  | Leggett Street | withdraw         | 30     |
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+-----+----------------+------+-------+-----+----------------+------------------+--------+


select *
from phone_calls
where caller = '(286) 555-6063'
-- RESULT
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 46  | (286) 555-6063 | (789) 555-8833 | 2023 | 7     | 25  | 125      |
-- | 254 | (286) 555-6063 | (676) 555-6554 | 2023 | 7     | 28  | 43       | ***
-- | 284 | (286) 555-6063 | (310) 555-8568 | 2023 | 7     | 28  | 235      |
-- | 451 | (286) 555-6063 | (344) 555-9601 | 2023 | 7     | 30  | 154      |
-- +-----+----------------+----------------+------+-------+-----+----------+

select *
from people
where phone_number = '(375) 555-8161'
-- RESULT
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 250277 | James | (676) 555-6554 | 2438825627      | Q13SVG6       |
-- +--------+-------+----------------+-----------------+---------------+

select *
from passengers
join flights on passengers.flight_id = flights.id
-- where passengers.passport_number = 2438825627;
where passengers.passport_number = 1988161715;
-- JAMES
-- +-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
-- | flight_id | passport_number | seat | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
-- | 10        | 2438825627      | 7C   | 10 | 8                 | 4                      | 2023 | 7     | 30  | 13   | 55     |
-- | 21        | 2438825627      | 6A   | 21 | 3                 | 8                      | 2023 | 7     | 26  | 17   | 58     |
-- | 47        | 2438825627      | 9B   | 47 | 4                 | 8                      | 2023 | 7     | 30  | 9    | 46     |
-- +-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
-- TAYLOR
-- +-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
-- | flight_id | passport_number | seat | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
-- | 36        | 1988161715      | 6D   | 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
-- +-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+

select *
from airports
where id = 8 and id = 4 and id = 3;
