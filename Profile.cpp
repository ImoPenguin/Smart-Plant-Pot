#include <Arduino.h>

using namespace std;

class Profile{
	private:
		String name;

		long Min_idealTemp;
		long Max_idealTemp;

		long Min_idealHumidity;
		long Max_idealHumidity;

		long Min_ideal_soilMoisture;
		long Max_ideal_soilMoisture;

	public:
		Profile(){
			name = "";

			Min_idealTemp = 0;
			Max_idealTemp = 0;

			Min_idealHumidity = 0;
			Max_idealHumidity = 0;

			Min_ideal_soilMoisture = 0;
			Max_ideal_soilMoisture = 0;
		}

		void set_IdealCondition(String Plant_Name, long min_Temp, long max_Temp, long min_Humidity, long max_Humidity, long min_soilMoisture, long max_soilMoisture){
			set_Name(Plant_Name);
			set_idealTemp(min_Temp, max_Temp);
			set_idealHumidity(min_Humidity, max_Humidity);
			set_ideal_soilMoisture(min_soilMoisture, max_soilMoisture);
		}

		String get_Name(){
			return name;
		}

		long get_Min_idealTemp(){
			return Min_idealTemp;
		}

		long get_Max_idealTemp(){
            return Max_idealTemp;
        }

		long get_Min_idealHumidity(){
			return Min_idealHumidity;
		}

		long get_Max_idealHumidity(){
            return Max_idealHumidity;
        }

		long get_Min_ideal_soilMoisture(){
            return Min_ideal_soilMoisture;
        }

		long get_Max_ideal_soilMoisture(){
            return Max_ideal_soilMoisture;
        }

		void set_Name(String Plant_Name){
			this->name = Plant_Name;
		}

		void set_idealTemp(long min_Temp, long max_Temp){
            this->Min_idealTemp = min_Temp;
			this->Max_idealTemp = max_Temp;
        }

		void set_idealHumidity(long min_Humidity, long max_Humidity){
            this->Min_idealHumidity = min_Humidity;
			this->Max_idealHumidity = max_Humidity;
        }

		void set_ideal_soilMoisture(long min_soilMoisture, long max_soilMoisture){
            this->Min_ideal_soilMoisture = min_soilMoisture;
			this->Max_ideal_soilMoisture = max_soilMoisture;
        }

		boolean isIdeal(long Temp, long Humidity, long soilMoisture){
			if(Temp < Min_idealTemp || Temp > Max_idealTemp){
				return false;
			}
			else if(Humidity < Min_idealHumidity || Humidity > Max_idealHumidity){
				return false;
			}
			else if(soilMoisture < Min_ideal_soilMoisture || soilMoisture > Max_ideal_soilMoisture){
                return false;
            }
			return true;
		}

};
