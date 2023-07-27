#include <Arduino.h>

#include <rgb_lcd.h>
#include <grove_two_rgb_led_matrix.h>
#include <DHT.h>

#include "Profile.cpp"

#define buttonPin 2
#define tempHumidPin 5
#define soilMoisturePin A0

long buttonState = 0;
long soilMoisture = 0;
long plantProfile = 1;

long counter;

bool isIdealCondition = true;

rgb_lcd lcd;
GroveTwoRGBLedMatrixClass matrix;
DHT dht(tempHumidPin, DHT11); // setting pin and sensor type

Profile *profiles = new Profile[3];

void setup()
{
	Serial.begin(4800);
	counter = 0;
	//  PROFILE for Snake Plants
	profiles[0].set_IdealCondition("Default", 18, 25, 20, 60, 300, 600);
	profiles[1].set_IdealCondition("Pothos", 15, 30, 20, 60, 400, 600);
	profiles[2].set_IdealCondition("Cactus", 20, 30, 25, 45, 50, 250);

	lcd.begin(16, 2);
	lcd.print("  Hello, World!  ");
	matrix.displayEmoji(21, 1000, true); // default to tree emoji on start-up for 2s
	delay(2000);
}

void loop()
{
	soilMoisture = analogRead(soilMoisturePin); // read soil moisture 0-300 = dry, 300-700 Moist, 700-950 wet

	float tempHumVal[2] = {0, 0}; // temperature and humidity is read as an array of float values | 0 = humidity, 1 = temp
	dht.readTempAndHumidity(tempHumVal);

	lcd.clear();

	// Humidity on LCD
	lcd.print(round(tempHumVal[0])); // rounding to remove unneccesary decimal point (Values are always read to nearest long anyways eg 24.00)
	lcd.print("% Humid");

	lcd.setCursor(12, 0); // move cursor to type on the top right
	// Temperature on LCD
	lcd.print(round(tempHumVal[1]));
	lcd.print(char(223)); // special character for degree symbol
	lcd.print("C");

	lcd.setCursor(0, 1); // move cursor to second row
	// Moisture on LCD
	lcd.print("Moisture: ");
	lcd.print(soilMoisture);

	// Cycle through 3 plant profiles on LCD
	buttonState = digitalRead(buttonPin);
	lcd.setCursor(14, 1);
	lcd.print("P");
	if (buttonState == HIGH)
	{
		if (plantProfile == 3)
		{
			plantProfile = 1;
		}
		else
		{
			plantProfile++;
		}
	}

	lcd.print(plantProfile);

	//	GET the plant Profile
	Profile p = profiles[plantProfile - 1];
	if (p.isIdeal(round(tempHumVal[1]), round(tempHumVal[0]), soilMoisture))
	{
		isIdealCondition = true;
		matrix.displayEmoji(1, 1000, true); // 1 = happy face
	}
	else
	{
		isIdealCondition = false;
		matrix.displayEmoji(2, 1000, true); // 2 = sad face
	}

	/***********************************************************************************************************

							!!!!!!		DISPLAY the SPECIFIC condition that is not ideal every 10s	!!!!!!

	************************************************************************************************************/

	//	if (isIdealCondition) == TRUE => skip condition check
	if (counter == 20)
	{
		if (isIdealCondition == true)
		{
			lcd.setRGB(255, 255, 255); // set colour to white
			counter = 0;
		}
		else
		{
			long temp = round(tempHumVal[1]);
			long humid = round(tempHumVal[0]);
			long soilMoist = soilMoisture;

			// setting colour
			long conditionsWrong = 0;
			if (temp < p.get_Min_idealTemp() || temp > p.get_Max_idealTemp())
			{
				conditionsWrong++;
			}
			if (humid < p.get_Min_idealHumidity() || humid > p.get_Max_idealHumidity())
			{
				conditionsWrong++;
			}
			if (soilMoist < p.get_Min_ideal_soilMoisture() || soilMoist > p.get_Max_ideal_soilMoisture())
			{
				conditionsWrong++;
			}

			switch (conditionsWrong)
			{
			case 1:
				lcd.setRGB(255, 200, 0);
				break;
			case 2:
				lcd.setRGB(255, 70, 0);
				break;
			case 3:
				lcd.setRGB(255, 0, 0);
				break;
			default:
				lcd.setRGB(255, 255, 255);
			}

			//  CHECK Temp
			if (temp < p.get_Min_idealTemp() || temp > p.get_Max_idealTemp())
			{
				lcd.clear();

				if (temp < p.get_Min_idealTemp())
				{
					lcd.print("  ! Low Temp !");
					lcd.setCursor(0, 1);
					lcd.print(" Currently: ");
					lcd.print(temp);
					lcd.print(char(223)); // special character for degree symbol
					lcd.print("C");
				}
				else
				{
					lcd.print("  ! High Temp !");
					lcd.print("  Currently: ");
					lcd.print(temp);
					lcd.print(char(223)); // special character for degree symbol
					lcd.print("C");
				}
				delay(2000);

				// SHOW recommended condition
				lcd.clear();
				lcd.print("  Proper Temp: ");

				lcd.setCursor(0, 1); // move cursor to second row
				lcd.print("  " + String(p.get_Min_idealTemp()) + char(223) + "C ~ " + String(p.get_Max_idealTemp()) + char(223) + "C");
				delay(2000);
			}

			//	CHECK Humidity
			if (humid < p.get_Min_idealHumidity() || humid > p.get_Max_idealHumidity())
			{
				lcd.clear();

				if (humid < p.get_Min_idealHumidity())
				{
					lcd.print("! Low Humidity !");
					lcd.setCursor(0, 1);
					lcd.print(" Currently: ");
					lcd.print(humid);
					lcd.print("%");
				}
				else
				{
					lcd.print("! High Humidity !");
				}
				delay(2000);

				// SHOW recommended condition
				lcd.clear();
				lcd.print("Proper Humidity: ");

				lcd.setCursor(0, 1); // move cursor to second row
				lcd.print("   " + String(p.get_Min_idealHumidity()) + "% ~ " + String(p.get_Max_idealHumidity()) + "%");
				delay(2000);
			}

			//    CHECK Soil Moisture
			if (soilMoist < p.get_Min_ideal_soilMoisture() || soilMoist > p.get_Max_ideal_soilMoisture())
			{
				lcd.clear();

				if (soilMoist < p.get_Min_ideal_soilMoisture())
				{
					lcd.print("! Low Moisutre !");
					lcd.setCursor(0, 1);
					lcd.print(" Currently: ");
					lcd.print(soilMoist);
				}
				else
				{
					lcd.print("!High Moisture!");
					lcd.setCursor(0, 1);
					lcd.print(" Currently: ");
					lcd.print(soilMoist);
				}
				delay(2000);

				// SHOW recommended condition
				lcd.clear();
				lcd.print("Proper Moisture: ");

				lcd.setCursor(0, 1); // move cursor to second row
				lcd.print("   " + String(p.get_Min_ideal_soilMoisture()) + " ~ " + String(p.get_Max_ideal_soilMoisture()));
				delay(2000);
			}

			//	RESET COUNTER
			counter = 0;
		}
	}
	Serial.println(counter);
	counter++;
	delay(10);
}

/*
CURRENT PROBLEMS

- Soil moisture is not measured by any unit. Its input it just an long that we need to determine a range ourselves for if a plant needs watering
		- from stack exchange
		All you can say from the numbers is "This is wet" or "This is dry". Normally you would use trial and error to determine what the values roughly relate to, and say that "Anything under X should trigger watering. Anything over Y should stop watering."
- Plant Profiles (DONE)
- Water Pump?

---		Future Development		---

- New custom Profiles can be added by Users
*/

// RGB Matrix code
/*************************************************************
Description
Display emoji on LED matrix.
Parameter
emoji: Set a number from 0 to 29 for different emoji.
0 smile 	10 heart 			20 house
1 laugh 	11 small heart 		21 tree
2 sad 		12 broken heart 	22 flower
3 mad 		13 waterdrop 		23 umbrella
4 angry 	14 flame 			24 rain
5 cry 		15 creeper 			25 monster
6 greedy 	16 mad creeper 		26 crab
7 cool 		17 sword 			27 duck
8 shy 		18 wooden sword 	28 rabbit
9 awkward 	19 crystal sword 	29 cat
30 up 		31 down 			32 left
33 right 	34 smile face
duration_time: Set the display time(ms) duration. Set it to 0 to not display.
forever_flag: Set it to true to display forever, and the duration_time will not work.
Or set it to false to display one time.
Return
Null.
************************************************************/