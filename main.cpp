#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

const vector<string> rType = { "add", "sub", "and", "or", "xor", "sll", "srl", "sra", "slt", "sltu" };
const vector<string> iType = { "lb", "lbu", "lh", "lhu", "lw", "addi", "slti", "sltiu", "xori", "ori", "andi", "slli", "srli", "srai", "jalr" };
const vector<string> sType = { "sb", "sh", "sw" };

const string opcodeR = "0110011";
const string opcodeS = "0100011";
const string opcodeLoad = "0000011";
const string opcodeI = "0100011";

string decimalToBinary( string );
string fillZero( string& );//FOR RD & RS
string immFillZero( string& );//FOR IMMEDIATE
void processR( string, string, string, string, string& );
void processS( string, string, string, string& );
void processLoad( string, string, string, string& );
void processI( string, string, string, string, string& );
void processSI( string, string, string, string, string& );

int main()
{
    
    ifstream file;
    file.open( "project1.txt", ios::in );
    
    if( !file )
    {
        cerr << "file could not open!\n";
        exit( 1 );
    }
    
    string buf;
    vector<string> firstData;
    while( file >> buf )
    {
        for( size_t i = 0; i < buf.size(); i++ )
        {
            if( buf[ i ] == ',' )
                buf[ i ] = ' ';
        }
    
        stringstream ss( buf );
        string s;
        while( ss >> s )
            firstData.push_back( s );
    }
    
    /*for( auto&str : firstData )
        cout << str << '\n';*/
    
    
    //START TO DETERMINE---------------------------------------------------
    
    for( size_t i = 0; i < firstData.size(); i++ )
    {
        string mCode = "";
        for( size_t r = 0; r < rType.size(); r++ )//R-Type
        {
            if( firstData[ i ] == rType[ r ] )
            {
                processR( firstData[ i ], firstData[ i + 1 ], firstData[ i + 2 ], firstData[ i + 3 ], mCode );
                i += 3;
            }
        }
        
        if( mCode != "" )
            continue;
        
        
        for( size_t s = 0; s < sType.size(); s++ )//S-Type----------
        {
            if( firstData[ i ] == sType[ s ] )
            {
                processS( firstData[ i ], firstData[ i + 1 ], firstData[ i + 2 ], mCode );
                i += 2;
            }
        }
        
        if( mCode != "" )
            continue;
        
        for( size_t _i = 0; _i < iType.size(); _i++ )//I-Type---------
        {
            if( firstData[ i ] == iType[ _i ] )
            {
                if( firstData[ i ] == "lb" || firstData[ i ] == "lbu" || firstData[ i ] == "lh" || firstData[ i ] == "lhu" || firstData[ i ] == "lw" )
                {
                    processLoad( firstData[ i ], firstData[ i + 1 ], firstData[ i + 2 ], mCode );
                    i += 2;
                }
                else if( firstData[ i ] == "slli" || firstData[ i ] == "srli" || firstData[ i ] == "srai" )
                {
                    processSI( firstData[ i ], firstData[ i + 1 ], firstData[ i + 2 ], firstData[ i + 3 ], mCode );
                    i += 3;
                }
                else
                {
                    processI( firstData[ i ], firstData[ i + 1 ], firstData[ i + 2 ], firstData[ i + 3 ], mCode );
                    i += 3;
                }
            }
        }
        
    }
    
    return 0;
}


string decimalToBinary( string _n )
{
    int a;
    string str;
    bool sign = true;
    
    if( _n[ 0 ] == '-' )
    {
        _n.erase( 0, 1 );
        sign = false;
    }
    
    int n = stoi( _n );
    
    for( int i = 0; n > 0; i++ )
    {
        a = n % 2;
        
        if( a == 1 )
            str += '1';
        else str += '0';
        
        n = n / 2;
    }
    
    reverse( str.begin(), str.end() );
    
    if( sign == false )
    {
        for( size_t i = 0; i < str.size(); i++ )
        {
            if( str[ i ] == '0' )
                str[ i ] = '1';
            else str[ i ] = '0';
        }
        
        if( str[ str.size() - 1 ] == '0' )
            str[ str.size() - 1 ] = '1';
        else
        {
            size_t t = str.size() - 1;
            while( str[ t ] == '1' )
            {
                str[ t ] = '0';
                t--;
            }
            if( t >= 0 )
                str[ t ] = '1';
        }
        if( str.size() < 12 )
            str.insert( str.begin(), '-' );
    }
    
    return str;
}

string fillZero( string &r )
{
    
    if( r.size() == 5 )
        return r;
    else if( r.size() < 5 )
        while( r.size() < 5)
            r.insert( r.begin(), '0' );
    
    return r;
}

string immFillZero( string &i )
{
    if( i.size() == 12 )
        return i;
    else if( i.size() < 12 )
    {
        if( i[ 0 ] != '-' )
            while( i.size() < 12 )
                i.insert( i.begin(), '0' );
        else //FILL 1
        {
            i.erase( 0, 1 );
            while( i.size() < 12 )
                i.insert( i.begin(), '1' );
        }
    }
    
    
    return i;
}

void processR( string a, string rd, string rs1, string rs2, string &m )//add rd rs1 rs2
{
    if( a == "sub" || a == "sra" )
        m = "0100000 ";
    else m = "0000000 ";
    
    rs2.erase( 0, 1 );//ERASE 'x'
    string _rs2 = decimalToBinary( rs2 );
    fillZero( _rs2 );
    m += _rs2;
    m += " ";
    
    rs1.erase( 0, 1 );//ERASE 'x'
    string _rs1 = decimalToBinary( rs1 );
    fillZero( _rs1 );
    m += _rs1;
    m += " ";
    
    string fun3 = "";
    if( a == "add" || a == "sub" )
        fun3 = "000";
    else if( a == "and" )
        fun3 = "111";
    else if( a == "or" )
        fun3 = "110";
    else if( a == "xor" )
        fun3 = "100";
    else if( a == "sll" )
        fun3 = "001";
    else if( a == "srl" || a == "sra" )
        fun3 = "101";
    else if( a == "slt" )
        fun3 = "010";
    else if( a == "sltu" )
        fun3 = "011";
    m += fun3;
    m += " ";
    
    rd.erase( 0, 1 );//ERASE 'x'
    string _rd = decimalToBinary( rd );
    fillZero( _rd );
    m += _rd;
    m += " ";
    
    m += opcodeR;
    
    
    cout << m << '\n';
}

void processS( string a, string rs2, string rs1, string &m )
{
    string imm;
 
    for( size_t i = 0; rs1[ i ] != '('; i++ )
        imm.insert( imm.begin() + i, rs1[ i ] );
    
    imm = decimalToBinary( imm );
    immFillZero( imm );
    string imm1;
    for( size_t i = 0; i < 7; i++ )
        imm1.insert( imm1.begin() + i, imm[ i ] );
    
    string imm2;
    for( size_t i = 7, j = 0; i < imm.size(); i++, j++ )
        imm2.insert( imm2.begin() + j, imm[ i ] );
    //FINISH IMMEDIATE PROCESSING---------------------------
    
    m += imm1;
    m += " ";
    
    rs2.erase( 0, 1 );//ERASE 'x'
    string _rs2 = decimalToBinary( rs2 );
    fillZero( _rs2 );
    m += _rs2;
    m += " ";
    
    size_t n = 0;//IGNORE ( )
    while( rs1[ n ] != '(' )
        n++;
    n++;
    string _rs1;
    for( size_t i = 0; n < rs1.size() - 1; i++, n++ )
        _rs1.insert( _rs1.begin() + i, rs1[ n ] );
    
    _rs1.erase( 0, 1 );//ERASE 'x'
    string __rs1 = decimalToBinary( _rs1 );
    fillZero( __rs1 );
    m += __rs1;
    m += " ";
    
    string fun3 = "";
    if( a == "sb" )
        fun3 = "000";
    else if( a == "sh" )
        fun3 = "001";
    else if( a == "sw" )
        fun3 = "010";
    m += fun3;
    m += " ";
    
    m += imm2;
    m += " ";
    
    m += opcodeS;
    
    cout << m << '\n';
}

void processLoad( string a, string rd, string offset, string &m )
{
    string imm;
    for( size_t i = 0; offset[ i ] != '('; i++ )
        imm.insert( imm.begin() + i, offset[ i ] );
    
    imm = decimalToBinary( imm );
    immFillZero( imm );
    m += imm;
    m += " ";
 
    
    size_t n = 0;//IGNORE ( )
    while( offset[ n ] != '(' )
        n++;
    n++;
    string _off;
    for( size_t i = 0; n < offset.size() - 1; i++, n++ )
        _off.insert( _off.begin() + i, offset[ n ] );
    
    _off.erase( 0, 1 );//ERASE 'x'
    string __off = decimalToBinary( _off );
    fillZero( __off );
    m += __off;
    m += " ";
    
    
    
    string fun3 = "";
    if( a == "lb" )
        fun3 = "000";
    else if( a == "lbu" )
        fun3 = "100";
    else if( a == "lh" )
        fun3 = "001";
    else if( a == "lhu" )
        fun3 = "101";
    else if( a == "lw" )
        fun3 = "010";
    m += fun3;
    m += " ";
    
    
    rd.erase( 0, 1 );//ERASE 'x'
    string _rd = decimalToBinary( rd );
    fillZero( _rd );
    m += _rd;
    m += " ";
    
    m += opcodeLoad;
    
    cout << m << '\n';
}


void processI( string a, string rd, string rs1, string imm, string &m )
{
    imm = decimalToBinary( imm );
    immFillZero( imm );
    m += imm;
    m += " ";
    
    rs1.erase( 0, 1 );//ERASE 'x'
    string _rs1 = decimalToBinary( rs1 );
    fillZero( _rs1 );
    m += _rs1;
    m += " ";
    
    
    string fun3 = "";
    if( a == "addi" || a == "jalr" )
        fun3 = "000";
    else if( a == "slti" )
        fun3 = "010";
    else if( a == "sltiu" )
        fun3 = "011";
    else if( a == "xori" )
        fun3 = "100";
    else if( a == "ori" )
        fun3 = "110";
    else if( a == "andi" )
        fun3 = "111";
    m += fun3;
    m += " ";
    
    
    rd.erase( 0, 1 );//ERASE 'x'
    string _rd = decimalToBinary( rd );
    fillZero( _rd );
    m += _rd;
    m += " ";
    
    
    if( a != "jalr" )
        m += opcodeI;
    else m += "1100111";
    
    
    cout << m << '\n';
}


void processSI( string a, string rd, string rs1, string imm, string &m )
{
    if( a == "srai" )
        m = "0100000 ";
    else m = "0000000 ";
    
    imm = decimalToBinary( imm );
    fillZero( imm );
    m += imm;
    m += " ";
    
    
    rs1.erase( 0, 1 );//ERASE 'x'
    string _rs1 = decimalToBinary( rs1 );
    fillZero( _rs1 );
    m += _rs1;
    m += " ";
    
    
    string fun3 = "";
    if( a == "slli" )
        fun3 = "001";
    else
        fun3 = "101";
    m += fun3;
    m += " ";
    
    
    rd.erase( 0, 1 );//ERASE 'x'
    string _rd = decimalToBinary( rd );
    fillZero( _rd );
    m += _rd;
    m += " ";
    
    m += "0010011";
    
    cout << m << '\n';
}
