/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/
/*
MaSzyna EU07 - SPKS
Brakes.
Copyright (C) 2007-2014 Maciej Cierniak
*/
#include "stdafx.h"
#include "mctools.h"
#include "Globals.h"

/*================================================*/

int ConversionError = 0;
int LineCount = 0;
bool DebugModeFlag = false;
bool FreeFlyModeFlag = false;

//std::string Ups(std::string s)
//{
//    int jatka;
//    std::string swy;
//
//    swy = "";
//    {
//        long jatka_end = s.length() + 1;
//        for (jatka = 0; jatka < jatka_end; jatka++)
//            swy = swy + UpCase(s[jatka]);
//    }
//    return swy;
//} /*=Ups=*/

int Max0(int x1, int x2)
{
    if (x1 > x2)
        return x1;
    else
        return x2;
}

int Min0(int x1, int x2)
{
    if (x1 < x2)
        return x1;
    else
        return x2;
}

double Max0R(double x1, double x2)
{
    if (x1 > x2)
        return x1;
    else
        return x2;
}

double Min0R(double x1, double x2)
{
    if (x1 < x2)
        return x1;
    else
        return x2;
}

// shitty replacement for Borland timestamp function
// TODO: replace with something sensible
std::string Now() {

    std::time_t timenow = std::time( nullptr );
    std::tm tm = *std::localtime( &timenow );
    std::stringstream  converter;
    converter << std::put_time( &tm, "%c" );
    return converter.str();
}

bool TestFlag(int Flag, int Value)
{
    if ((Flag & Value) == Value)
        return true;
    else
        return false;
}

bool SetFlag(int &Flag, int Value)
{
	return iSetFlag(Flag, Value);
}

bool iSetFlag(int &Flag, int Value)
{
    if (Value > 0)
    {
        if ((Flag & Value) == 0)
        {
            Flag |= Value;
            return true; // true, gdy było wcześniej 0 i zostało ustawione
        }
    }
    else if (Value < 0)
    {
        Value = abs(Value);
        if ((Flag & Value) == Value)
        {
            Flag &= ~Value; // Value jest ujemne, czyli zerowanie flagi
            return true; // true, gdy było wcześniej 1 i zostało wyzerowane
        }
    }
	return false;
}

bool UnSetFlag(int &Flag, int Value)
{
	Flag &= ~Value;
	return true;
}

inline double Random(double a, double b)
{
    std::uniform_real_distribution<> dis(a, b);
    return dis(Global::random_engine);
}

bool FuzzyLogic(double Test, double Threshold, double Probability)
{
    if ((Test > Threshold) && (!DebugModeFlag))
        return
            (Random() < Probability * Threshold * 1.0 / Test) /*im wiekszy Test tym wieksza szansa*/;
    else
        return false;
}

bool FuzzyLogicAI(double Test, double Threshold, double Probability)
{
    if ((Test > Threshold))
        return
            (Random() < Probability * Threshold * 1.0 / Test) /*im wiekszy Test tym wieksza szansa*/;
    else
        return false;
}

std::string DUE(std::string s) /*Delete Before Equal sign*/
{
    //DUE = Copy(s, Pos("=", s) + 1, length(s));
	return s.substr(s.find("=") + 1, s.length());
}

std::string DWE(std::string s) /*Delete After Equal sign*/
{
    size_t ep = s.find("=");
	if (ep != std::string::npos)
		//DWE = Copy(s, 1, ep - 1);
		return s.substr(0, ep);
    else
        return s;
}

std::string ExchangeCharInString( std::string const &Source, char const &From, char const &To )
{
	std::string replacement; replacement.reserve( Source.size() );
	std::for_each(Source.cbegin(), Source.cend(), [&](char const idx) {
		if( idx != From )    { replacement += idx; }
		else {
			if( To != NULL ) { replacement += To; } }
	} );

	return replacement;
}

std::vector<std::string> &Split(const std::string &s, char delim, std::vector<std::string> &elems)
{ // dzieli tekst na wektor tekstow

    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> Split(const std::string &s, char delim)
{ // dzieli tekst na wektor tekstow
    std::vector<std::string> elems;
    Split(s, delim, elems);
    return elems;
}

std::vector<std::string> Split(const std::string &s)
{ // dzieli tekst na wektor tekstow po białych znakach
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (ss >> item)
	{
		elems.push_back(item);
	}
	return elems;
}

std::string to_string(int _Val)
{
	std::ostringstream o;
	o << _Val;
	return o.str();
};

std::string to_string(unsigned int _Val)
{
	std::ostringstream o;
	o << _Val;
	return o.str();
};

std::string to_string(double _Val)
{
	std::ostringstream o;
	o << _Val;
	return o.str();
};

std::string to_string(int _Val, int precision)
{
	std::ostringstream o;
	o << std::fixed << std::setprecision(precision);
	o << _Val;
	return o.str();
};

std::string to_string(double _Val, int precision)
{
	std::ostringstream o;
	o << std::fixed << std::setprecision(precision);
	o << _Val;
	return o.str();
};

std::string to_string(int _Val, int precision, int width)
{
	std::ostringstream o;
	o.width(width);
	o << std::fixed << std::setprecision(precision);
	o << _Val;
	return o.str();
};

std::string to_string(double const Value, int const Precision, int const Width)
{
	std::ostringstream converter;
	converter << std::setw( Width ) << std::fixed << std::setprecision(Precision) << Value;
	return converter.str();
};

std::string to_hex_str( int const Value, int const Width )
{
	std::ostringstream converter;
	converter << "0x" << std::uppercase << std::setfill( '0' ) << std::setw( Width ) << std::hex << Value;
	return converter.str();
};

int stol_def(const std::string &str, const int &DefaultValue)
{
    int result { DefaultValue };
    std::stringstream converter;
    converter << str;
    converter >> result;
    return result;
/*
	// this function was developed iteratively on Codereview.stackexchange
	// with the assistance of @Corbin
	std::size_t len = str.size();
	while (std::isspace(str[len - 1]))
		len--;
	if (len == 0)
		return DefaultValue;
	errno = 0;
	char *s = new char[len + 1];
	std::strncpy(s, str.c_str(), len);
	char *p;
	int result = strtol(s, &p, 0);
	delete[] s;
	if( ( *p != '\0' ) || ( errno != 0 ) )
	{
		return DefaultValue;
	}
	return result;
*/
}

std::string ToLower(std::string const &text)
{
	std::string lowercase( text );
	std::transform(text.begin(), text.end(), lowercase.begin(), ::tolower);
	return lowercase;
}

std::string ToUpper(std::string const &text)
{
	std::string uppercase( text );
	std::transform(text.begin(), text.end(), uppercase.begin(), ::toupper);
	return uppercase;
}

// replaces polish letters with basic ascii
void
win1250_to_ascii( std::string &Input ) {

    std::unordered_map<char, char> charmap{
        { 165, 'A' }, { 198, 'C' }, { 202, 'E' }, { 163, 'L' }, { 209, 'N' }, { 211, 'O' }, { 140, 'S' }, { 143, 'Z' }, { 175, 'Z' },
        { 185, 'a' }, { 230, 'c' }, { 234, 'e' }, { 179, 'l' }, { 241, 'n' }, { 243, 'o' }, { 156, 's' }, { 159, 'z' }, { 191, 'z' }
    };
    std::unordered_map<char, char>::const_iterator lookup;
    for( auto &input : Input ) {
        if( ( lookup = charmap.find( input ) ) != charmap.end() )
            input = lookup->second;
    }
}

void ComputeArc(double X0, double Y0, double Xn, double Yn, double R, double L, double dL,
                double &phi, double &Xout, double &Yout)
/*wylicza polozenie Xout Yout i orientacje phi punktu na elemencie dL luku*/
{
    double dX;
    double dY;
    double Xc; 
    double Yc;
    double gamma;
    double alfa;
    double AbsR;

    if ((R != 0) && (L != 0))
    {
        AbsR = abs(R);
        dX = Xn - X0;
        dY = Yn - Y0;
        if (dX != 0)
            gamma = atan(dY * 1.0 / dX);
        else if (dY > 0)
            gamma = M_PI * 1.0 / 2;
        else
            gamma = 3 * M_PI * 1.0 / 2;
        alfa = L * 1.0 / R;
        phi = gamma - (alfa + M_PI * Round(R * 1.0 / AbsR)) * 1.0 / 2;
        Xc = X0 - AbsR * cos(phi);
        Yc = Y0 - AbsR * sin(phi);
        phi = phi + alfa * dL * 1.0 / L;
        Xout = AbsR * cos(phi) + Xc;
        Yout = AbsR * sin(phi) + Yc;
    }
}

void ComputeALine(double X0, double Y0, double Xn, double Yn, double L, double R, double &Xout,
                  double &Yout)
{
    double dX;
    double dY;
    double gamma;
    double alfa;
    /*   pX,pY : real;*/

    alfa = 0; // ABu: bo nie bylo zainicjowane
    dX = Xn - X0;
    dY = Yn - Y0;
    if (dX != 0)
        gamma = atan(dY * 1.0 / dX);
    else if (dY > 0)
        gamma = M_PI * 1.0 / 2;
    else
        gamma = 3 * M_PI * 1.0 / 2;
    if (R != 0)
        alfa = L * 1.0 / R;
    Xout = X0 + L * cos(alfa * 1.0 / 2 - gamma);
    Yout = Y0 + L * sin(alfa * 1.0 / 2 - gamma);
}

bool FileExists( std::string const &Filename ) {

	std::ifstream file( Filename );
    return( true == file.is_open() );
}

/*
//graficzne:

double Xhor(double h)
{
    return h * Hstep + Xmin;
}

double Yver(double v)
{
    return (Vsize - v) * Vstep + Ymin;
}

long Horiz(double x)
{
    x = (x - Xmin) * 1.0 / Hstep;
    if (x > -INT_MAX)
        if (x < INT_MAX)
            return Round(x);
        else
            return INT_MAX;
    else
        return -INT_MAX;
}

long Vert(double Y)
{
    Y = (Y - Ymin) * 1.0 / Vstep;
    if (Y > -INT_MAX)
        if (Y < INT_MAX)
            return Vsize - Round(Y);
        else
            return INT_MAX;
    else
        return -INT_MAX;
}
*/

// NOTE: this now does nothing.
void ClearPendingExceptions()
// resetuje błędy FPU, wymagane dla Trunc()
{
    ; /*?*/ /* ASM
       FNCLEX
 ASM END */
}

// END
