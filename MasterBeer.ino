/*
 Name:		MasterBeer.ino
 Created:	02.05.2019 12:24:28
 Author:	UglyRedFrog 
 Email:		milkevych@ukr.net
*/

#include <URTouch.h>
#include <UTFT.h>
#include <DallasTemperature-3.8.0/DallasTemperature.h>

UTFT myGLCD(ILI9341_16, 38, 39, 40, 41);	// підключення екрану на пінах
URTouch  myTouch(6, 5, 4, 3, 2);			// підключення тачскріну на пінах

const uint8_t maxPause = 5;		// Максимальна кількість пауз(задається програмістом)

extern uint8_t BigFont[];		// підключення шрифту

uint8_t Pause = NULL;			// кількість пауз(значення для створення масиву PauseArr) 
uint16_t PauseArr [maxPause];	// масив довжиною maxPause, і містить у собі значення з температурними паузами.
uint16_t arrTimer[maxPause];	// масив довжиною maxPause, і містить у собі значення з часом температурних пауз.


struct Button
{
	Button() //стандартний Конструктор установлює ПУСТІ розміри та символ кнопки
	{
		uint16_t X1 = NULL;
		uint16_t Y1 = NULL;
		uint16_t X2 = NULL;
		uint16_t Y2 = NULL;
		uint8_t symbol = NULL;
	}

	Button(uint16_t X, uint16_t Y, uint16_t length, uint16_t height, uint8_t symbol) //Конструктор з ЗАДАНИМИ параметрами кнопки			
	{
		this->X1 = X;
		this->Y1 = Y;
		this->X2 = X + length;
		this->Y2 = Y + height;
		this->symbol = symbol;
	};

	void SETBUTTON() //Функція МАЛЮВАННЯ ОБ'ЄКТУ на екрані (створений конструктором об'єкт - кнопка).
	{
		//myGLCD.fillRect(this->X1, this->Y1, this->X2, this->Y2 );
		myGLCD.fillRoundRect(this->X1, this->Y1, this->X2, this->Y2);
		myGLCD.printChar(this->symbol, (this->X1 + this->X2) / 2, (this->Y1 + this->Y2) / 2);
		
	}
	void setAll(uint16_t X, uint16_t Y, uint16_t length, uint16_t height, uint8_t symbol)
		//Функція УСТАНОВЛЕННЯ ПАРАМЕТРІВ об'єкту (створеної конструктором - об'єкт кнопка),
												//(точки відліку Х,У; ширини; висоти; та символу;).*/
	
	{
		this->X1 = X;
		this->Y1 = Y;
		this->X2 = X + length;
		this->Y2 = Y + height;
		this->symbol = symbol;
	}

	void setPoint(uint16_t X, uint16_t Y) 
		//Функція УСТАНОВЛЕННЯ ПАРАМЕТРІВ об'єкту (створеної конструктором - об'єкт кнопка),
											    //(точки відліку Х,У).*/
		{
			this->X1 = X;
			this->Y1 = Y;
		}
	void setLength(uint16_t length)
		//Функція УСТАНОВЛЕННЯ ПАРАМЕТРУ об'єкту (створеної конструктором - об'єкт кнопка),
											//( ШИРИНИ;).*/
	{

		this->X2 = this->X1 + length;
	
	}
	void setHeight(uint16_t height)
		//Функція УСТАНОВЛЕННЯ ПАРАМЕТРУ об'єкту (створеної конструктором - об'єкт кнопка),
										//( ВИСОТИ;).*/
	{
		this->Y2 = this->Y1 + height;
	}
	void setSymbol(uint8_t symbol)	
		//Функція УСТАНОВЛЕННЯ ПАРАМЕТРУ об'єкту (створеної конструктором - об'єкт кнопка),
												//( СИМВОЛУ;).*/
	{
		this->symbol = symbol;
	}

	// Гетери значень кнопки
	uint16_t getX1() {
		return this->X1;
	}
	uint16_t getX2() {
		return this->X2;
	}
	uint16_t getY1() {
		return this->Y1;
	}
	uint16_t getY2() {
		return this->Y2;
	}

	// Параметри кнопки
private:
	uint16_t X1 = NULL; // Поч. точ. з ліва на право.
	uint16_t Y1 = NULL; // Поч. точ. з гори на низ.
	uint16_t X2 = NULL; // Кін. точ. з ліва на право.
	uint16_t Y2 = NULL; // Кін. точ. з гори на низ.
	uint8_t symbol = NULL; // Символ у кнопці.

};

struct Grafic
	// Конструктор обєкту графік
{
	Grafic(uint16_t arrPoint[], uint16_t arrTimer[]) 
	{
		myGLCD.setColor(VGA_BLUE);						// Колір шкали синій

		myGLCD.drawHLine(20, 220, myGLCD.getDisplayXSize() - 25);				/* Лінія шкали ЧАСУ (Горизонтальна)
														
														(Поч. точ. з ліва на право - Х1), 
														(Кін. точ. з ліва на право - Х2), 
														(Поч. точ. з гори на низ - Y1), 
														(Кін. точ. з гори на низ - Y2)*/

		myGLCD.drawVLine(20, 5, myGLCD.getDisplayYSize() - 25);				/* Лінія шкали ТЕМПЕРАТУРИ (Вертикальна)
														
														(Поч. точ. з ліва на право - Х1), 
														(Кін. точ. з ліва на право - Х2), 
														(Поч. точ. з гори на низ - Y1), 
														(Кін. точ. з гори на низ - Y2)*/
		//myGLCD.drawVLine()
		
		myGLCD.print("Time, min", CENTER, 221); // Підпис лінії, шкали ЧАСУ (Горизонтальна) 
		myGLCD.print("Temperature,`C", 0, myGLCD.getDisplayYSize() - 20, 270); // Підпис лінії, шкали ТЕМПЕРАТУРИ (Вертикальна) 
		
		//for (uint8_t i = 0; i < Pause; i++)
		//{
		//	myGLCD.drawHLine(/* число що означаэ выдлык по Х - arrPoint[i]*/,/* число що означаэ выдлык по У - arrPoint[i]*/, arrTimer[i]);
		//}

		myGLCD.setColor(VGA_RED); // Колір лінії графіка на червоний

		for (uint8_t i = 0; i < Pause; i++)
		{
			SummArrTimer = SummArrTimer + arrTimer[i]; // Рахуємо суму часу тривалості всіх пауз 
			//( для коректного відображення графіку, в подальшому використовується для обчислення маштабу)
		}
		if ((myGLCD.getDisplayXSize() - 25) > SummArrTimer)//обчислення маштабу
		{
			scale = ((float)(myGLCD.getDisplayXSize() - 25)) / ((float)SummArrTimer); // якщо екран більший від сумарної тривалості пауз,
																					  // то маштаб робимо на збільшення відмальовування графіку
		}
		else {
			scale = ((float)SummArrTimer)/ ((float)(myGLCD.getDisplayXSize() - 25));  // якщо екран меньший від сумарної тривалості пауз,
																					  // то маштаб робимо на зменшення відмальовування графіку
		}
		
		// малюємо графік
		for (uint8_t i = 0; i < Pause; i++)
		{
			myGLCD.drawHLine(score,
				myGLCD.getDisplayYSize() - 25 - arrPoint[i], arrTimer[i] * scale);
			score = score + arrTimer[i] * scale;
		}

		myGLCD.setColor(VGA_BLUE); // Повертаємо колір лінії на синій
	}; 
private:
	uint16_t SummArrTimer = 0;
	float scale = 0;
	uint16_t score = 20;  //лічильник по Х, для початку наступної лінії
};

struct Page // обєкт сторінка сторінка яка відображається
{
	Page()
	{

	};

	void SetPage() // метод обєкту очищає сторінку і заливає її кольром
	{ 
		myGLCD.clrScr();
		myGLCD.fillScr(VGA_LIME);
	}

};


void setup() {
	myGLCD.InitLCD(LANDSCAPE);		// Ініціалізація положення екрану горизонально
	myTouch.InitTouch(LANDSCAPE);	// Ініціалізація положення тачскріна горизонально

	myGLCD.setBackColor(VGA_LIME);	// Установка кольору заднього тла
	myGLCD.setColor(VGA_BLUE);		// Установка коліру тексту
	myGLCD.setFont(BigFont);		// Установка розміру шрифту
	
	myTouch.setPrecision(PREC_EXTREME); /////////////////////?????????????????????????????????????????????????????????

}
Button Plus = Button();		// Обєкт кнопка +
Button Minus = Button();	// Обєкт кнопка -
Button Back = Button();		// Обєкт кнопка <
Button Next = Button();		// Обєкт кнопка >

Page page = Page();			// Обєкn сторінка


void loop()
{
	////////////////////////////////////////////////////////////////////////////////////
	/////				Intro	---- first page										////
	////////////////////////////////////////////////////////////////////////////////////
	link1:																						// Силка на першу стрінку - для кнопки " < " на другій сторінці
	page.SetPage();																				// Метод обєкта очищає сторінку
	myGLCD.print("MasterBeer", CENTER, myGLCD.getDisplayYSize() * 0.30 - 20);					// Назва програми
	myGLCD.print("Creator:", CENTER, myGLCD.getDisplayYSize() * 0.30);							// Розробник
	myGLCD.print("Mil'kevych A.A.", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);				// Імя розробника
	myGLCD.print("Boguslav city.", CENTER, myGLCD.getDisplayYSize() * 0.80 - 20);				// Місто розробки
	myGLCD.print("Ukraine", CENTER, myGLCD.getDisplayYSize() * 0.80);							// Країна розробки
	myGLCD.print("2019 year", CENTER, myGLCD.getDisplayYSize() * 0.80 + 20);					// Рік розробки
	
	////////////////////////////////////////////////////////////////////////////////////
	/////				цикл обробки натискань - провірка роботи екрану				////
	////////////////////////////////////////////////////////////////////////////////////

	while (true) // постійно провіряє екран на доторки
	{ 
		if (myTouch.dataAvailable()) {
			myGLCD.print("Scsess!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 60); //
			// якщо працює виводить "Scsess!!!" у центрі. 
			break;
		}
	};

	delay(1000); ///// ?????????????????????????? перевірити чи потрібна затримка тут

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////				Next page	---- two page																				////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
link2:											// Силка на другу стрінку для кнопки < на третій сторінці


	 page.SetPage();							// Метод обєкта очищає сторінку
	 Plus.setAll(10, 140, 145, 40, '+');        // Установлюємо параметри для кнопки +
	 Minus.setAll(165, 140, 145, 40, '-');		// Установлюємо параметри для кнопки -
	 Back.setAll(10, 190, 145, 40, '<');		// Установлюємо параметри для кнопки <
	 Next.setAll(165, 190, 145, 40, '>');		// Установлюємо параметри для кнопки >

	Plus.SETBUTTON();		// Малюємо кнопку + на екрані
	Minus.SETBUTTON();		// Малюємо кнопку - на екрані
	Back.SETBUTTON();		// Малюємо кнопку < на екрані
	Next.SETBUTTON();		// Малюємо кнопку > на екрані


	////////////////////////////////////////////////////////////////////////////////////
	/////				цикл обробки натискань - кількості пауз						////
	////////////////////////////////////////////////////////////////////////////////////

	myGLCD.print("Koli4estvo pause", 30, 0, 0); // надпис для юзера кількість пауз
	myGLCD.print("dl9L zatopy", 70, 20, 0); // надпис для юзера кількість пауз

link_touch_Pause: // силка на початок обробки натискань для кількості пауз

	myGLCD.printNumI(Pause, 155, 110, 0); // виводить значення Pause на екран в теперішньому стані

	while (true) // постійно провіряє екран на доторки
	{
		if (myTouch.dataAvailable()) // якщо є доторк
		{
			myTouch.read(); // читаємо значення точки доторку
		}

		// перевірка, якщо точка доторку попадає межі кнопки Plus
		if ((myTouch.getX() >= Plus.getX1()) &&
			(myTouch.getX() <= Plus.getX2()) &&
			(myTouch.getY() >= Plus.getY1()) &&
			(myTouch.getY() <= Plus.getY2()))
		{
			myGLCD.print("Plus!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20); // пишемо Plus
			delay(250);
			myTouch.TP_X = NULL; // очищаємо записані значення функцією myTouch.read();
			myTouch.TP_Y = NULL; // очищаємо записані значення функцією myTouch.read();
			Pause++;			// інкремент Pause
			goto link_touch_Pause;	// повертаємося до перевірки екрану на доторки
			//break;
		}
		// перевірка, якщо точка доторку попадає межі кнопки Minus
		// логіка та сама, що і в кнопці Plus
		if ((myTouch.getX() >= Minus.getX1()) &&
			(myTouch.getX() <= Minus.getX2()) &&
			(myTouch.getY() >= Minus.getY1()) &&
			(myTouch.getY() <= Minus.getY2()))
		{
			myGLCD.print("Minus!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
			delay(250);
			myTouch.TP_X = NULL;
			myTouch.TP_Y = NULL;
			Pause--;
			goto link_touch_Pause;
			//break;
		}
		// перевірка, якщо точка доторку попадає межі кнопки Next
		// логіка та сама, що і в кнопці Plus
		if ((myTouch.getX() >= Next.getX1()) &&
			(myTouch.getX() <= Next.getX2()) &&
			(myTouch.getY() >= Next.getY1()) &&
			(myTouch.getY() <= Next.getY2()))
		{
			myGLCD.print("Next!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
			delay(250);
			myTouch.TP_X = NULL;
			myTouch.TP_Y = NULL;
			break;
		}
		// перевірка, якщо точка доторку попадає межі кнопки Back
		// логіка та сама, що і в кнопці Plus, але результат обробки - повернення на першу сторінку, а не на провірку дотику.
		if ((myTouch.getX() >= Back.getX1()) &&
			(myTouch.getX() <= Back.getX2()) &&
			(myTouch.getY() >= Back.getY1()) &&
			(myTouch.getY() <= Back.getY2()))
		{
			myGLCD.print("Back!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
			delay(250);
			myTouch.TP_X = NULL;
			myTouch.TP_Y = NULL;
			Pause = NULL;
			goto link1;								//повернення та титульну сторінку(перша сторінка з лого)
		}

	}
	//////////////закынчення циклу///////////////////////////
	//////захист від дурака Pause не може бутити менше 0, а також не може бути великим числом(кнопки не вміщуватимуться на екран)
	if (Pause == 0 || Pause < 0 || Pause > maxPause)
	{
		goto link2; // повернення до сторінки 2, - де заново переписуємо кількість пауз у Pause
	}

	//////////////////////////////////////////////////////////////////////////
	//		початок третьої сторінки	 -----	вибір температурниї пауз	/////
	//////////////////////////////////////////////////////////////////////////*/
	
link3:	// Силка на третю стрінку - для кнопки " < " на четвертій сторінці

	// створюэмо масиви кнопок та запонвюємо масив пауз
	
	page.SetPage();									// Метод обєкта очищає сторінку
	
	// створюємо вказівник на масив кнопок кількістю Pause
	Button* arrButtPlus = new Button[Pause];		// + кнопка на конкретну паузу
	Button* arrButtMinus = new Button[Pause];		// - кнопка на конкретну паузу
	
	//заповнюємо масив PauseArr, значеннями темперетурами кількістю Pause
	for (uint8_t i = 0; i < Pause; i++)
	{
		PauseArr[i] = 40 + i * 10; // кожну наступну паузу збільшуємо на 10 градусів
	}
	/////////////////// розстановка кнопок //////////////////////

	/*для кожної паузи із масиву PauseArr 
	з кількістю в значень Pause в масиві, 
	встановлюємо дві кнопки + та - для регулювання значень у масиві PauseArr*/
	
	for (uint8_t i = 0; i < Pause; i++)
	{
		arrButtPlus[i] = { Button(4, 4 + (39 * i), 100, 35, '+') }; // Визиваємо Конструктор з ЗАДАНИМИ параметрами кнопки
		arrButtPlus[i].SETBUTTON();									// Малюємо кнопку
		arrButtMinus[i] = { Button(114, 4 + (39 * i), 100, 35, '-') };
		arrButtMinus[i].SETBUTTON();
			if (i == Pause-1) // після розташування останньої паузи, малюємо функціональні кнопки
			{
				Back.SETBUTTON(); // Малюємо кнопку
				Next.SETBUTTON(); // Малюємо кнопку
			}
	}
	
	////////////////////////////////////////////////////////////////////////////////
	//		цикл обробки натискань - установка значень температурних пауз		////
    // аналогічно як для сторінки 2												////
	////////////////////////////////////////////////////////////////////////////////

link_touch_PauseArr: // силка на початок обробки натискань параметрів температури пауз

	
	while (true)
	{
		if (myTouch.dataAvailable())
		{
			myTouch.read();
		}
		for (uint8_t i = 0; i < Pause; i++)
		{
			myGLCD.printNumI(PauseArr[i], 114 + 110, 4 + (39 * i), 0);
			if ((myTouch.getX() >= arrButtPlus[i].getX1()) &&
				(myTouch.getX() <= arrButtPlus[i].getX2()) &&
				(myTouch.getY() >= arrButtPlus[i].getY1()) &&
				(myTouch.getY() <= arrButtPlus[i].getY2()))
			{
				myGLCD.print("Plus!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
				delay(250);
				myTouch.TP_X = NULL;
				myTouch.TP_Y = NULL;
				PauseArr[i]++;
				goto link_touch_PauseArr;
			}
			if ((myTouch.getX() >= arrButtMinus[i].getX1()) &&
				(myTouch.getX() <= arrButtMinus[i].getX2()) &&
				(myTouch.getY() >= arrButtMinus[i].getY1()) &&
				(myTouch.getY() <= arrButtMinus[i].getY2()))
			{
				myGLCD.print("Minus!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
				delay(250);
				myTouch.TP_X = NULL;
				myTouch.TP_Y = NULL;
				PauseArr[i]--;
				goto link_touch_PauseArr;
			}
		}
		if ((myTouch.getX() >= Next.getX1()) &&
			(myTouch.getX() <= Next.getX2()) &&
			(myTouch.getY() >= Next.getY1()) &&
			(myTouch.getY() <= Next.getY2()))
		{
			myGLCD.print("Next!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
			delay(250);
			myTouch.TP_X = NULL;
			myTouch.TP_Y = NULL;
			delete[] arrButtPlus, arrButtMinus; // очищаємо пам'ять від створеного масиву кнопок
			break;
		}
		if ((myTouch.getX() >= Back.getX1()) &&
			(myTouch.getX() <= Back.getX2()) &&
			(myTouch.getY() >= Back.getY1()) &&
			(myTouch.getY() <= Back.getY2()))
		{
			myGLCD.print("Back!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
			delay(250);
			myTouch.TP_X = NULL;
			myTouch.TP_Y = NULL;
			delete[] arrButtPlus, arrButtMinus; // очищаємо пам'ять від створеного масиву кнопок
			Pause = NULL;
			goto link2;
		}
	}

		////////////закінчення циклу/////////////*

	/////////////////////////////////////////////////////////////////////////////////////////
	//		початок четвертої сторінки	 -----	установка тривалості температурних пауз	/////
	/////////////////////////////////////////////////////////////////////////////////////////

	link4: // Силка на 4ту стрінку - для кнопки " < " на 5тій сторінці

		// створюэмо масиви кнопок та запонвюємо масив зчасом для витримки пауз

	page.SetPage();									// Метод обєкта очищає сторінку

	// створюємо вказівник на масив кнопок кількістю Pause 
	Button* arrButtPlus1 = new Button[Pause];		// + кнопка на конкретну паузу
	Button* arrButtMinus1 = new Button[Pause];		// - кнопка на конкретну паузу

////////////////////////	цикл заповнення часу витримки пауз

	//заповнюємо масив arrTimer, значеннями темперетурами кількістю Pause

	for (uint8_t i = 0; i < Pause; i++)
	{
		arrTimer[i] = 10 + i * 10; // кожну наступну паузу збільшуємо на 10 хвилин
	}
	/////////////////// розстановка кнопок //////////////////////

	/*для кожної паузи із масиву PauseArr
	з кількістю в значень Pause в масиві,
	встановлюємо дві кнопки + та - для регулювання значень у масиві PauseArr*/

	for (uint8_t i = 0; i < Pause; i++)
	{
		arrButtPlus1[i] = { Button(4, 4 + (39 * i), 100, 35, '+') }; // Визиваємо Конструктор з ЗАДАНИМИ параметрами кнопки
		arrButtPlus1[i].SETBUTTON();									// Малюємо кнопку
		arrButtMinus1[i] = { Button(114, 4 + (39 * i), 100, 35, '-') };
		arrButtMinus1[i].SETBUTTON();
		if (i == Pause - 1) // після розташування останньої паузи, малюємо функціональні кнопки
		{
			Back.SETBUTTON(); // Малюємо кнопку
			Next.SETBUTTON(); // Малюємо кнопку
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	//		цикл обробки натискань - установка тривалості температурних пауз	////
    // аналогічно як для сторінки 2												////
	////////////////////////////////////////////////////////////////////////////////

link_touch_arrTimer:	// силка на початок обробки натискань параметрів часу температурних пауз

	while (true)
	{
		if (myTouch.dataAvailable())
		{
			myTouch.read();
		}
		for (uint8_t i = 0; i < Pause; i++)
		{
			myGLCD.printNumI(arrTimer[i], 114 + 110, 4 + (39 * i),2);

			if ((myTouch.getX() >= arrButtPlus[i].getX1()) &&
				(myTouch.getX() <= arrButtPlus[i].getX2()) &&
				(myTouch.getY() >= arrButtPlus[i].getY1()) &&
				(myTouch.getY() <= arrButtPlus[i].getY2()))
			{
				myGLCD.print("Plus!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
				delay(250);
				myTouch.TP_X = NULL;
				myTouch.TP_Y = NULL;
				arrTimer[i]++;
				goto link_touch_arrTimer;
			}

			if ((myTouch.getX() >= arrButtMinus[i].getX1()) &&
				(myTouch.getX() <= arrButtMinus[i].getX2()) &&
				(myTouch.getY() >= arrButtMinus[i].getY1()) &&
				(myTouch.getY() <= arrButtMinus[i].getY2()))
			{
				myGLCD.print("Minus!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
				delay(250);
				myTouch.TP_X = NULL;
				myTouch.TP_Y = NULL;
				arrTimer[i]--;
				goto link_touch_arrTimer;
			}
		}

		if ((myTouch.getX() >= Next.getX1()) &&
			(myTouch.getX() <= Next.getX2()) &&
			(myTouch.getY() >= Next.getY1()) &&
			(myTouch.getY() <= Next.getY2()))
		{
			myGLCD.print("Next!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
			delay(250);
			myTouch.TP_X = NULL;
			myTouch.TP_Y = NULL;
			delete[] arrButtPlus1, arrButtMinus1; // очищаємо пам'ять від створеного масиву кнопок
			break;
		}

		if ((myTouch.getX() >= Back.getX1()) &&
			(myTouch.getX() <= Back.getX2()) &&
			(myTouch.getY() >= Back.getY1()) &&
			(myTouch.getY() <= Back.getY2()))
		{
			myGLCD.print("Back!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
			delay(250);
			myTouch.TP_X = NULL;
			myTouch.TP_Y = NULL;
			delete[] arrButtPlus1, arrButtMinus1; // очищаємо пам'ять від створеного масиву кнопок
			goto link3;
		}
	}
	////////////// закінчення циклу

	/////////////////////////////////////////////////////////////////////////////////////////
	//		початок п'ятої сторінки	 -----	список температурних пауз та їх тривалість	/////
	/////////////////////////////////////////////////////////////////////////////////////////

link5:		// Силка на 4ту стрінку для кнопки < на 5 сторінці

	page.SetPage();							// Метод обєкта очищає сторінку

	for (size_t i = 0, j = 1; i < Pause; i++, j++)
	{
		myGLCD.print("#", 0, 0 + i * 20, 0);
		myGLCD.printNumI(j, 15, 0 + i * 20, 2);
		myGLCD.print("Heat", 60, 0 + i * 20, 0);
		myGLCD.printNumI(PauseArr[i], 160, 0 + i * 20, 2);
		myGLCD.print("Time", 200, 0 + i * 20, 0);
		myGLCD.printNumI(arrTimer[i], 280, 0 + i * 20, 2);
	}


	Back.setAll(10, 190, 145, 40, '<');		// Установлюємо параметри для кнопки <
	Next.setAll(165, 190, 145, 40, '>');		// Установлюємо параметри для кнопки >

	Back.SETBUTTON();		// Малюємо кнопку < на екрані
	Next.SETBUTTON();		// Малюємо кнопку > на екрані
	
////////////////////////////////////////////////////////////////////////////////////
/////				цикл обробки натискань - списку пауз						////
////////////////////////////////////////////////////////////////////////////////////

	while (true) // постійно провіряє екран на доторки
	{
		if (myTouch.dataAvailable()) // якщо є доторк
		{
			myTouch.read(); // читаємо значення точки доторку
		}

		// перевірка, якщо точка доторку попадає межі кнопки Next
		// логіка та сама, що і в кнопці Plus
		if ((myTouch.getX() >= Next.getX1()) &&
			(myTouch.getX() <= Next.getX2()) &&
			(myTouch.getY() >= Next.getY1()) &&
			(myTouch.getY() <= Next.getY2()))
		{
			myGLCD.print("Next!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
			delay(250);
			myTouch.TP_X = NULL;
			myTouch.TP_Y = NULL;
			break;
		}
		// перевірка, якщо точка доторку попадає межі кнопки Back
		// логіка та сама, що і в кнопці Plus, але результат обробки - повернення на першу сторінку, а не на провірку дотику.
		if ((myTouch.getX() >= Back.getX1()) &&
			(myTouch.getX() <= Back.getX2()) &&
			(myTouch.getY() >= Back.getY1()) &&
			(myTouch.getY() <= Back.getY2()))
		{
			myGLCD.print("Back!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
			delay(250);
			myTouch.TP_X = NULL;
			myTouch.TP_Y = NULL;
			goto link4;								//повернення та титульну сторінку(перша сторінка з лого)
		}

	}

//////////////закынчення циклу///////////////////////////
//////захист від дурака Pause не може бутити менше 0, а також не може бути великим числом(кнопки не вміщуватимуться на екран)
if (Pause == 0 || Pause < 0 || Pause > maxPause)
{
	goto link2; // повернення до сторінки 2, - де заново переписуємо кількість пауз у Pause
}

		/////////////////////////////////////////////////////////////////////////////////////////
		//		початок шостої сторінки	 -----	графік		затирання						/////
		/////////////////////////////////////////////////////////////////////////////////////////

page.SetPage();

	Grafic one = Grafic(PauseArr, arrTimer);

	//pinMode(9, OUTPUT);
	


	while (true)
	{
		//digitalWrite(9, HIGH);

		if (myTouch.dataAvailable()) {
			myGLCD.print("Scsess!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 60);
			break;
		}
	};

	delay(1000); ///// ?????????????????????????? перевірити чи потрібна затримка тут
	/////////////////////////////////////////////////////////////////////////////////////////
	//		початок сьомої сторінки	 -----							/////
	/////////////////////////////////////////////////////////////////////////////////////////
	
	page.SetPage();

	myGLCD.print("Time, min", LEFT, 0); // Підпис лінії, шкали ЧАСУ (Горизонтальна) 
	myGLCD.print("Temperature,`C", LEFT, myGLCD.getDisplayYSize() * 0.30 + 35); // Підпис лінії, шкали ТЕМПЕРАТУРИ (Вертикальна) 

	DallasTemperature T = DallasTemperature();
	T.begin();
	
	myGLCD.printNumI(T.getDeviceCount(),CENTER,0,'.');
	delay(1000); ///// ?????????????????????????? перевірити чи потрібна затримка тут
	myGLCD.printNumI(T.getDS18Count(), CENTER, 0, '.');

	delay(1000); ///// ?????????????????????????? перевірити чи потрібна затримка тут


	while (true)
	{
		//digitalWrite(9, HIGH);

		if (myTouch.dataAvailable()) {
			myGLCD.print("Scsess!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 60);
			break;
		}
	};

	delay(1000); ///// ?????????????????????????? перевірити чи потрібна затримка тут


	Pause = NULL;

}



//myGLCD.print("MasterBeer", CENTER, myGLCD.getDisplayYSize()* 0.30 - 20);
//myGLCD.print("Creator:", CENTER, myGLCD.getDisplayYSize()* 0.30);
//myGLCD.print("Mil'kevych A.A.", CENTER, myGLCD.getDisplayYSize()* 0.30 + 20);
//myGLCD.print("Boguslav city.", CENTER, myGLCD.getDisplayYSize()* 0.80 - 20);
//myGLCD.print("Ukraine", CENTER, myGLCD.getDisplayYSize()* 0.80);
//myGLCD.print("2019 year", CENTER, myGLCD.getDisplayYSize()* 0.80 + 20);
