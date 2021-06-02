/*
 Name:		MasterBeer.ino
 Created:	02.05.2019 12:24:28
 Author:	UglyRedFrog 
 Email:		milkevych@ukr.net
*/

#include <URTouch.h>
#include <UTFT.h>
#include <DallasTemperature-3.8.0/DallasTemperature.h>

UTFT myGLCD(ILI9341_16, 38, 39, 40, 41);	// ���������� ������ �� ����
URTouch  myTouch(6, 5, 4, 3, 2);			// ���������� �������� �� ����

const uint8_t maxPause = 5;		// ����������� ������� ����(�������� �����������)

extern uint8_t BigFont[];		// ���������� ������

uint8_t Pause = NULL;			// ������� ����(�������� ��� ��������� ������ PauseArr) 
uint16_t PauseArr [maxPause];	// ����� �������� maxPause, � ������ � ��� �������� � �������������� �������.
uint16_t arrTimer[maxPause];	// ����� �������� maxPause, � ������ � ��� �������� � ����� ������������� ����.


struct Button
{
	Button() //����������� ����������� ���������� ���Ҳ ������ �� ������ ������
	{
		uint16_t X1 = NULL;
		uint16_t Y1 = NULL;
		uint16_t X2 = NULL;
		uint16_t Y2 = NULL;
		uint8_t symbol = NULL;
	}

	Button(uint16_t X, uint16_t Y, uint16_t length, uint16_t height, uint8_t symbol) //����������� � �������� ����������� ������			
	{
		this->X1 = X;
		this->Y1 = Y;
		this->X2 = X + length;
		this->Y2 = Y + height;
		this->symbol = symbol;
	};

	void SETBUTTON() //������� ��������� ��'���� �� ����� (��������� ������������� ��'��� - ������).
	{
		//myGLCD.fillRect(this->X1, this->Y1, this->X2, this->Y2 );
		myGLCD.fillRoundRect(this->X1, this->Y1, this->X2, this->Y2);
		myGLCD.printChar(this->symbol, (this->X1 + this->X2) / 2, (this->Y1 + this->Y2) / 2);
		
	}
	void setAll(uint16_t X, uint16_t Y, uint16_t length, uint16_t height, uint8_t symbol)
		//������� ������������ �������в� ��'���� (�������� ������������� - ��'��� ������),
												//(����� ����� �,�; ������; ������; �� �������;).*/
	
	{
		this->X1 = X;
		this->Y1 = Y;
		this->X2 = X + length;
		this->Y2 = Y + height;
		this->symbol = symbol;
	}

	void setPoint(uint16_t X, uint16_t Y) 
		//������� ������������ �������в� ��'���� (�������� ������������� - ��'��� ������),
											    //(����� ����� �,�).*/
		{
			this->X1 = X;
			this->Y1 = Y;
		}
	void setLength(uint16_t length)
		//������� ������������ ��������� ��'���� (�������� ������������� - ��'��� ������),
											//( ������;).*/
	{

		this->X2 = this->X1 + length;
	
	}
	void setHeight(uint16_t height)
		//������� ������������ ��������� ��'���� (�������� ������������� - ��'��� ������),
										//( ������;).*/
	{
		this->Y2 = this->Y1 + height;
	}
	void setSymbol(uint8_t symbol)	
		//������� ������������ ��������� ��'���� (�������� ������������� - ��'��� ������),
												//( �������;).*/
	{
		this->symbol = symbol;
	}

	// ������ ������� ������
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

	// ��������� ������
private:
	uint16_t X1 = NULL; // ���. ���. � ��� �� �����.
	uint16_t Y1 = NULL; // ���. ���. � ���� �� ���.
	uint16_t X2 = NULL; // ʳ�. ���. � ��� �� �����.
	uint16_t Y2 = NULL; // ʳ�. ���. � ���� �� ���.
	uint8_t symbol = NULL; // ������ � ������.

};

struct Grafic
	// ����������� ����� ������
{
	Grafic(uint16_t arrPoint[], uint16_t arrTimer[]) 
	{
		myGLCD.setColor(VGA_BLUE);						// ���� ����� ����

		myGLCD.drawHLine(20, 220, myGLCD.getDisplayXSize() - 25);				/* ˳�� ����� ���� (�������������)
														
														(���. ���. � ��� �� ����� - �1), 
														(ʳ�. ���. � ��� �� ����� - �2), 
														(���. ���. � ���� �� ��� - Y1), 
														(ʳ�. ���. � ���� �� ��� - Y2)*/

		myGLCD.drawVLine(20, 5, myGLCD.getDisplayYSize() - 25);				/* ˳�� ����� ����������� (�����������)
														
														(���. ���. � ��� �� ����� - �1), 
														(ʳ�. ���. � ��� �� ����� - �2), 
														(���. ���. � ���� �� ��� - Y1), 
														(ʳ�. ���. � ���� �� ��� - Y2)*/
		//myGLCD.drawVLine()
		
		myGLCD.print("Time, min", CENTER, 221); // ϳ���� ��, ����� ���� (�������������) 
		myGLCD.print("Temperature,`C", 0, myGLCD.getDisplayYSize() - 20, 270); // ϳ���� ��, ����� ����������� (�����������) 
		
		//for (uint8_t i = 0; i < Pause; i++)
		//{
		//	myGLCD.drawHLine(/* ����� �� ������� ������ �� � - arrPoint[i]*/,/* ����� �� ������� ������ �� � - arrPoint[i]*/, arrTimer[i]);
		//}

		myGLCD.setColor(VGA_RED); // ���� �� ������� �� ��������

		for (uint8_t i = 0; i < Pause; i++)
		{
			SummArrTimer = SummArrTimer + arrTimer[i]; // ������ ���� ���� ��������� ��� ���� 
			//( ��� ���������� ����������� �������, � ���������� ��������������� ��� ���������� �������)
		}
		if ((myGLCD.getDisplayXSize() - 25) > SummArrTimer)//���������� �������
		{
			scale = ((float)(myGLCD.getDisplayXSize() - 25)) / ((float)SummArrTimer); // ���� ����� ������ �� ������� ��������� ����,
																					  // �� ������ ������ �� ��������� �������������� �������
		}
		else {
			scale = ((float)SummArrTimer)/ ((float)(myGLCD.getDisplayXSize() - 25));  // ���� ����� ������� �� ������� ��������� ����,
																					  // �� ������ ������ �� ��������� �������������� �������
		}
		
		// ������� ������
		for (uint8_t i = 0; i < Pause; i++)
		{
			myGLCD.drawHLine(score,
				myGLCD.getDisplayYSize() - 25 - arrPoint[i], arrTimer[i] * scale);
			score = score + arrTimer[i] * scale;
		}

		myGLCD.setColor(VGA_BLUE); // ��������� ���� �� �� ����
	}; 
private:
	uint16_t SummArrTimer = 0;
	float scale = 0;
	uint16_t score = 20;  //�������� �� �, ��� ������� �������� ��
};

struct Page // ���� ������� ������� ��� ������������
{
	Page()
	{

	};

	void SetPage() // ����� ����� ����� ������� � ������ �� �������
	{ 
		myGLCD.clrScr();
		myGLCD.fillScr(VGA_LIME);
	}

};


void setup() {
	myGLCD.InitLCD(LANDSCAPE);		// ����������� ��������� ������ ������������
	myTouch.InitTouch(LANDSCAPE);	// ����������� ��������� �������� ������������

	myGLCD.setBackColor(VGA_LIME);	// ��������� ������� �������� ���
	myGLCD.setColor(VGA_BLUE);		// ��������� ����� ������
	myGLCD.setFont(BigFont);		// ��������� ������ ������
	
	myTouch.setPrecision(PREC_EXTREME); /////////////////////?????????????????????????????????????????????????????????

}
Button Plus = Button();		// ���� ������ +
Button Minus = Button();	// ���� ������ -
Button Back = Button();		// ���� ������ <
Button Next = Button();		// ���� ������ >

Page page = Page();			// ���n �������


void loop()
{
	////////////////////////////////////////////////////////////////////////////////////
	/////				Intro	---- first page										////
	////////////////////////////////////////////////////////////////////////////////////
	link1:																						// ����� �� ����� ������ - ��� ������ " < " �� ����� �������
	page.SetPage();																				// ����� ����� ����� �������
	myGLCD.print("MasterBeer", CENTER, myGLCD.getDisplayYSize() * 0.30 - 20);					// ����� ��������
	myGLCD.print("Creator:", CENTER, myGLCD.getDisplayYSize() * 0.30);							// ���������
	myGLCD.print("Mil'kevych A.A.", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);				// ��� ����������
	myGLCD.print("Boguslav city.", CENTER, myGLCD.getDisplayYSize() * 0.80 - 20);				// ̳��� ��������
	myGLCD.print("Ukraine", CENTER, myGLCD.getDisplayYSize() * 0.80);							// ����� ��������
	myGLCD.print("2019 year", CENTER, myGLCD.getDisplayYSize() * 0.80 + 20);					// г� ��������
	
	////////////////////////////////////////////////////////////////////////////////////
	/////				���� ������� ��������� - ������� ������ ������				////
	////////////////////////////////////////////////////////////////////////////////////

	while (true) // ������� ������� ����� �� �������
	{ 
		if (myTouch.dataAvailable()) {
			myGLCD.print("Scsess!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 60); //
			// ���� ������ �������� "Scsess!!!" � �����. 
			break;
		}
	};

	delay(1000); ///// ?????????????????????????? ��������� �� ������� �������� ���

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////				Next page	---- two page																				////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
link2:											// ����� �� ����� ������ ��� ������ < �� ����� �������


	 page.SetPage();							// ����� ����� ����� �������
	 Plus.setAll(10, 140, 145, 40, '+');        // ������������ ��������� ��� ������ +
	 Minus.setAll(165, 140, 145, 40, '-');		// ������������ ��������� ��� ������ -
	 Back.setAll(10, 190, 145, 40, '<');		// ������������ ��������� ��� ������ <
	 Next.setAll(165, 190, 145, 40, '>');		// ������������ ��������� ��� ������ >

	Plus.SETBUTTON();		// ������� ������ + �� �����
	Minus.SETBUTTON();		// ������� ������ - �� �����
	Back.SETBUTTON();		// ������� ������ < �� �����
	Next.SETBUTTON();		// ������� ������ > �� �����


	////////////////////////////////////////////////////////////////////////////////////
	/////				���� ������� ��������� - ������� ����						////
	////////////////////////////////////////////////////////////////////////////////////

	myGLCD.print("Koli4estvo pause", 30, 0, 0); // ������ ��� ����� ������� ����
	myGLCD.print("dl9L zatopy", 70, 20, 0); // ������ ��� ����� ������� ����

link_touch_Pause: // ����� �� ������� ������� ��������� ��� ������� ����

	myGLCD.printNumI(Pause, 155, 110, 0); // �������� �������� Pause �� ����� � ����������� ����

	while (true) // ������� ������� ����� �� �������
	{
		if (myTouch.dataAvailable()) // ���� � ������
		{
			myTouch.read(); // ������ �������� ����� �������
		}

		// ��������, ���� ����� ������� ������ ��� ������ Plus
		if ((myTouch.getX() >= Plus.getX1()) &&
			(myTouch.getX() <= Plus.getX2()) &&
			(myTouch.getY() >= Plus.getY1()) &&
			(myTouch.getY() <= Plus.getY2()))
		{
			myGLCD.print("Plus!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20); // ������ Plus
			delay(250);
			myTouch.TP_X = NULL; // ������� ������� �������� �������� myTouch.read();
			myTouch.TP_Y = NULL; // ������� ������� �������� �������� myTouch.read();
			Pause++;			// ��������� Pause
			goto link_touch_Pause;	// ����������� �� �������� ������ �� �������
			//break;
		}
		// ��������, ���� ����� ������� ������ ��� ������ Minus
		// ����� �� ����, �� � � ������ Plus
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
		// ��������, ���� ����� ������� ������ ��� ������ Next
		// ����� �� ����, �� � � ������ Plus
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
		// ��������, ���� ����� ������� ������ ��� ������ Back
		// ����� �� ����, �� � � ������ Plus, ��� ��������� ������� - ���������� �� ����� �������, � �� �� ������� ������.
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
			goto link1;								//���������� �� �������� �������(����� ������� � ����)
		}

	}
	//////////////���������� �����///////////////////////////
	//////������ �� ������ Pause �� ���� ������ ����� 0, � ����� �� ���� ���� ������� ������(������ �� �������������� �� �����)
	if (Pause == 0 || Pause < 0 || Pause > maxPause)
	{
		goto link2; // ���������� �� ������� 2, - �� ������ ���������� ������� ���� � Pause
	}

	//////////////////////////////////////////////////////////////////////////
	//		������� ������ �������	 -----	���� ������������ ����	/////
	//////////////////////////////////////////////////////////////////////////*/
	
link3:	// ����� �� ����� ������ - ��� ������ " < " �� �������� �������

	// ��������� ������ ������ �� ���������� ����� ����
	
	page.SetPage();									// ����� ����� ����� �������
	
	// ��������� �������� �� ����� ������ ������� Pause
	Button* arrButtPlus = new Button[Pause];		// + ������ �� ��������� �����
	Button* arrButtMinus = new Button[Pause];		// - ������ �� ��������� �����
	
	//���������� ����� PauseArr, ���������� ������������� ������� Pause
	for (uint8_t i = 0; i < Pause; i++)
	{
		PauseArr[i] = 40 + i * 10; // ����� �������� ����� �������� �� 10 �������
	}
	/////////////////// ����������� ������ //////////////////////

	/*��� ����� ����� �� ������ PauseArr 
	� ������� � ������� Pause � �����, 
	������������ �� ������ + �� - ��� ����������� ������� � ����� PauseArr*/
	
	for (uint8_t i = 0; i < Pause; i++)
	{
		arrButtPlus[i] = { Button(4, 4 + (39 * i), 100, 35, '+') }; // �������� ����������� � �������� ����������� ������
		arrButtPlus[i].SETBUTTON();									// ������� ������
		arrButtMinus[i] = { Button(114, 4 + (39 * i), 100, 35, '-') };
		arrButtMinus[i].SETBUTTON();
			if (i == Pause-1) // ���� ������������ �������� �����, ������� ������������ ������
			{
				Back.SETBUTTON(); // ������� ������
				Next.SETBUTTON(); // ������� ������
			}
	}
	
	////////////////////////////////////////////////////////////////////////////////
	//		���� ������� ��������� - ��������� ������� ������������� ����		////
    // ��������� �� ��� ������� 2												////
	////////////////////////////////////////////////////////////////////////////////

link_touch_PauseArr: // ����� �� ������� ������� ��������� ��������� ����������� ����

	
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
			delete[] arrButtPlus, arrButtMinus; // ������� ���'��� �� ���������� ������ ������
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
			delete[] arrButtPlus, arrButtMinus; // ������� ���'��� �� ���������� ������ ������
			Pause = NULL;
			goto link2;
		}
	}

		////////////��������� �����/////////////*

	/////////////////////////////////////////////////////////////////////////////////////////
	//		������� �������� �������	 -----	��������� ��������� ������������� ����	/////
	/////////////////////////////////////////////////////////////////////////////////////////

	link4: // ����� �� 4�� ������ - ��� ������ " < " �� 5�� �������

		// ��������� ������ ������ �� ���������� ����� ������ ��� �������� ����

	page.SetPage();									// ����� ����� ����� �������

	// ��������� �������� �� ����� ������ ������� Pause 
	Button* arrButtPlus1 = new Button[Pause];		// + ������ �� ��������� �����
	Button* arrButtMinus1 = new Button[Pause];		// - ������ �� ��������� �����

////////////////////////	���� ���������� ���� �������� ����

	//���������� ����� arrTimer, ���������� ������������� ������� Pause

	for (uint8_t i = 0; i < Pause; i++)
	{
		arrTimer[i] = 10 + i * 10; // ����� �������� ����� �������� �� 10 ������
	}
	/////////////////// ����������� ������ //////////////////////

	/*��� ����� ����� �� ������ PauseArr
	� ������� � ������� Pause � �����,
	������������ �� ������ + �� - ��� ����������� ������� � ����� PauseArr*/

	for (uint8_t i = 0; i < Pause; i++)
	{
		arrButtPlus1[i] = { Button(4, 4 + (39 * i), 100, 35, '+') }; // �������� ����������� � �������� ����������� ������
		arrButtPlus1[i].SETBUTTON();									// ������� ������
		arrButtMinus1[i] = { Button(114, 4 + (39 * i), 100, 35, '-') };
		arrButtMinus1[i].SETBUTTON();
		if (i == Pause - 1) // ���� ������������ �������� �����, ������� ������������ ������
		{
			Back.SETBUTTON(); // ������� ������
			Next.SETBUTTON(); // ������� ������
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	//		���� ������� ��������� - ��������� ��������� ������������� ����	////
    // ��������� �� ��� ������� 2												////
	////////////////////////////////////////////////////////////////////////////////

link_touch_arrTimer:	// ����� �� ������� ������� ��������� ��������� ���� ������������� ����

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
			delete[] arrButtPlus1, arrButtMinus1; // ������� ���'��� �� ���������� ������ ������
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
			delete[] arrButtPlus1, arrButtMinus1; // ������� ���'��� �� ���������� ������ ������
			goto link3;
		}
	}
	////////////// ��������� �����

	/////////////////////////////////////////////////////////////////////////////////////////
	//		������� �'��� �������	 -----	������ ������������� ���� �� �� ���������	/////
	/////////////////////////////////////////////////////////////////////////////////////////

link5:		// ����� �� 4�� ������ ��� ������ < �� 5 �������

	page.SetPage();							// ����� ����� ����� �������

	for (size_t i = 0, j = 1; i < Pause; i++, j++)
	{
		myGLCD.print("#", 0, 0 + i * 20, 0);
		myGLCD.printNumI(j, 15, 0 + i * 20, 2);
		myGLCD.print("Heat", 60, 0 + i * 20, 0);
		myGLCD.printNumI(PauseArr[i], 160, 0 + i * 20, 2);
		myGLCD.print("Time", 200, 0 + i * 20, 0);
		myGLCD.printNumI(arrTimer[i], 280, 0 + i * 20, 2);
	}


	Back.setAll(10, 190, 145, 40, '<');		// ������������ ��������� ��� ������ <
	Next.setAll(165, 190, 145, 40, '>');		// ������������ ��������� ��� ������ >

	Back.SETBUTTON();		// ������� ������ < �� �����
	Next.SETBUTTON();		// ������� ������ > �� �����
	
////////////////////////////////////////////////////////////////////////////////////
/////				���� ������� ��������� - ������ ����						////
////////////////////////////////////////////////////////////////////////////////////

	while (true) // ������� ������� ����� �� �������
	{
		if (myTouch.dataAvailable()) // ���� � ������
		{
			myTouch.read(); // ������ �������� ����� �������
		}

		// ��������, ���� ����� ������� ������ ��� ������ Next
		// ����� �� ����, �� � � ������ Plus
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
		// ��������, ���� ����� ������� ������ ��� ������ Back
		// ����� �� ����, �� � � ������ Plus, ��� ��������� ������� - ���������� �� ����� �������, � �� �� ������� ������.
		if ((myTouch.getX() >= Back.getX1()) &&
			(myTouch.getX() <= Back.getX2()) &&
			(myTouch.getY() >= Back.getY1()) &&
			(myTouch.getY() <= Back.getY2()))
		{
			myGLCD.print("Back!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 20);
			delay(250);
			myTouch.TP_X = NULL;
			myTouch.TP_Y = NULL;
			goto link4;								//���������� �� �������� �������(����� ������� � ����)
		}

	}

//////////////���������� �����///////////////////////////
//////������ �� ������ Pause �� ���� ������ ����� 0, � ����� �� ���� ���� ������� ������(������ �� �������������� �� �����)
if (Pause == 0 || Pause < 0 || Pause > maxPause)
{
	goto link2; // ���������� �� ������� 2, - �� ������ ���������� ������� ���� � Pause
}

		/////////////////////////////////////////////////////////////////////////////////////////
		//		������� ����� �������	 -----	������		���������						/////
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

	delay(1000); ///// ?????????????????????????? ��������� �� ������� �������� ���
	/////////////////////////////////////////////////////////////////////////////////////////
	//		������� ����� �������	 -----							/////
	/////////////////////////////////////////////////////////////////////////////////////////
	
	page.SetPage();

	myGLCD.print("Time, min", LEFT, 0); // ϳ���� ��, ����� ���� (�������������) 
	myGLCD.print("Temperature,`C", LEFT, myGLCD.getDisplayYSize() * 0.30 + 35); // ϳ���� ��, ����� ����������� (�����������) 

	DallasTemperature T = DallasTemperature();
	T.begin();
	
	myGLCD.printNumI(T.getDeviceCount(),CENTER,0,'.');
	delay(1000); ///// ?????????????????????????? ��������� �� ������� �������� ���
	myGLCD.printNumI(T.getDS18Count(), CENTER, 0, '.');

	delay(1000); ///// ?????????????????????????? ��������� �� ������� �������� ���


	while (true)
	{
		//digitalWrite(9, HIGH);

		if (myTouch.dataAvailable()) {
			myGLCD.print("Scsess!!!", CENTER, myGLCD.getDisplayYSize() * 0.30 + 60);
			break;
		}
	};

	delay(1000); ///// ?????????????????????????? ��������� �� ������� �������� ���


	Pause = NULL;

}



//myGLCD.print("MasterBeer", CENTER, myGLCD.getDisplayYSize()* 0.30 - 20);
//myGLCD.print("Creator:", CENTER, myGLCD.getDisplayYSize()* 0.30);
//myGLCD.print("Mil'kevych A.A.", CENTER, myGLCD.getDisplayYSize()* 0.30 + 20);
//myGLCD.print("Boguslav city.", CENTER, myGLCD.getDisplayYSize()* 0.80 - 20);
//myGLCD.print("Ukraine", CENTER, myGLCD.getDisplayYSize()* 0.80);
//myGLCD.print("2019 year", CENTER, myGLCD.getDisplayYSize()* 0.80 + 20);
