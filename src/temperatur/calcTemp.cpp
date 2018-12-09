float calculateTemperature(int raw){
	float temp = ((float)raw * (3.3/1024) -0.5) * 100;
	return temp;
}
