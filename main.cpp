// Linux   : g++ -o OpenOK main.cpp OpenOK/OpenOK.cpp -ldl -lusb-1.0
// Windows : g++ -o OpenOK main.cpp OpenOK/OpenOK.cpp libusb-1.0.dll


#include "OpenOK/OpenOK.h"
#include <stdio.h>
#include <memory>
#include <time.h>

//#define _DEBUG_

#ifdef _DEBUG_
#define debug_en true
#define str ". press enter to continue"
#else
#define debug_en false
#define str "."
#endif

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------

double diffclock( clock_t clock1, clock_t clock2 )
{
    double diffticks = clock1 - clock2;

    double diffms = ( diffticks * 10 ) / CLOCKS_PER_SEC;

    return diffms;
}

//---------------------------------------------------------------------------------------------------------------------------------

template <class OK>
void OK_Test( OK *xem )
{
    const int n = 100;
    string strDeviceID;
    int ret = -1;
    string BitFileName;
    bool dataReadCorrect;

    typename OK::BoardModel boardModel;

    unsigned char temp[ n ] = { 0, };

    BitFileName = "new_top.bit";

    printf( "%s%s","Load default PLL configuration", str );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }

    xem->LoadDefaultPLLConfiguration();

    printf( "%s%s","Get board model", str );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }

    boardModel = xem->GetBoardModel();

    printf( "Device model: %s\n", xem->GetBoardModelString(boardModel).c_str() );

    OpenOK_CPLL22150 *pll = new OpenOK_CPLL22150;

    printf( "%s\n","--------------------------------------------" );

    printf( "%s%s","GetEepromPLL22150Configuration()", str );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }

    xem->GetEepromPLL22150Configuration( *pll );

    printf( "GetReference(): %f\n", pll->GetReference() );
    printf( "GetVCOFrequency(): %f\n", pll->GetVCOFrequency() );
    printf( "GetVCOP(): %d\n", pll->GetVCOP() );
    printf( "GetVCOQ(): %d\n", pll->GetVCOQ() );
    printf( "GetDiv1Divider(): %d\n", pll->GetDiv1Divider() );
    printf( "GetDiv2Divider(): %d\n", pll->GetDiv2Divider() );
    printf( "GetDiv1Source(): %d\n", pll->GetDiv1Source() );
    printf( "GetDiv2Source(): %d\n", pll->GetDiv2Source() );

    for( int n = 0; n < 6; n++ ) {
        printf( "GetOutputFrequency(%d): %f\n", n, pll->GetOutputFrequency( n ) );
        printf( "GetOutputSource(%d): %d\n", n, pll->GetOutputSource( n ) );
    }

    printf( "%s\n","--------------------------------------------" );

    pll->SetReference( 48.0f, true );
    pll->SetVCOParameters( 25, 3 );
    pll->SetDiv1( OpenOK_CPLL22150::DivSrc_VCO, 4 );
    pll->SetOutputSource( 0, OpenOK_CPLL22150::ClkSrc_Div1ByN );
    pll->SetOutputEnable( 0, true );
    pll->SetDiv2( OpenOK_CPLL22150::DivSrc_VCO, 8 );
    pll->SetOutputSource( 1, OpenOK_CPLL22150::ClkSrc_Div2ByN );
    pll->SetOutputEnable( 1, true );

    printf( "%s%s", "SetPLL22150Configuration()", str );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }

    xem->SetPLL22150Configuration( *pll );

    printf( "%s\n","--------------------------------------------" );

    printf( "%s%s","GetPLL22150Configuration()", str );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }

    xem->GetPLL22150Configuration( *pll );

    printf( "GetReference(): %f\n", pll->GetReference() );
    printf( "GetVCOFrequency(): %f\n", pll->GetVCOFrequency() );
    printf( "GetVCOP(): %d\n", pll->GetVCOP() );
    printf( "GetVCOQ(): %d\n", pll->GetVCOQ() );
    printf( "GetDiv1Divider(): %d\n", pll->GetDiv1Divider() );
    printf( "GetDiv2Divider(): %d\n", pll->GetDiv2Divider() );
    printf( "GetDiv1Source(): %d\n", pll->GetDiv1Source() );
    printf( "GetDiv2Source(): %d\n", pll->GetDiv2Source() );

    for( int n = 0; n < 6; n++ ) {
        printf( "GetOutputFrequency(%d): %f\n", n, pll->GetOutputFrequency( n ) );
        printf( "GetOutputSource(%d): %d\n", n, pll->GetOutputSource( n ) );
    }

    printf( "%s\n","--------------------------------------------" );

    printf( "%s%s","Load information", str );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }

    printf( "Device firmware version: %d.%d\n", xem->GetDeviceMajorVersion(), xem->GetDeviceMinorVersion() );

    printf( "Device serial number: %s\n", xem->GetSerialNumber().c_str() );

    strDeviceID = xem->GetDeviceID();

    printf( "Device ID: %s\n", strDeviceID.c_str() );

    printf( "%s%s","Load bit file", str );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }

    ret = xem->ConfigureFPGA( BitFileName );

    if( ret >= 0 ) {
        printf( "Bit file \"%s\" loaded with success%s",BitFileName.c_str(), str );

        if( debug_en ) {
            getchar();
        } else {
            printf( "\n" );
        }
    } else {
        printf( "Bit file load failure%s", str );

        if( debug_en ) {
            getchar();
        } else {
            printf( "\n" );
        }
    }

    printf( "%s%s","Check for FrontPanel support in the FPGA configuration", str );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }

    printf( "Status FrontPanel : %s\n", xem->IsFrontPanelEnabled() ? "Enable" : "Disable" );

    printf( "Reset FPGA%s", str );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }

    //Reset FPGA
    xem->SetWireInValue( 0x00, 0xff, 0x01 );
    xem->UpdateWireIns();
    xem->SetWireInValue( 0x00, 0x00, 0x01 );
    xem->UpdateWireIns();

    xem->UpdateWireOuts();

    printf( "getAvailableToSend : %d\n", xem->GetWireOutValue( 0x20 ) );

    printf( "getAvailableToReceive : %d\n", xem->GetWireOutValue( 0x21 ) );

    printf( "getDebugWord_1 : %04x\n", ( unsigned short )xem->GetWireOutValue( 0x22 ) );

    printf( "getDebugWord_2 : %04x\n", ( unsigned short )xem->GetWireOutValue( 0x23 ) );

    int nWrites = 1;

    printf( "Wait for %d %s%s", nWrites, "WriteToPipeIn/ReadFromPipeOut", str );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }

    int h = 17;

    temp[ 0 ] = 7; temp[ n-3 ] = 13;temp[ n-1 ] = h;

    clock_t begin = clock();

    dataReadCorrect = false;

    for( int kj = 0 ; kj < nWrites; kj++ )
    {
        if( kj % 2 ) {
            h = 17;
        } else {
            h = 18;
        }

        temp[ n-1 ] = h;

        xem->WriteToPipeIn( 0x80, n, temp );

        temp[ 0 ] = 0; temp[ 1 ] = 0;temp[ n-1 ] = 0;

        xem->ReadFromPipeOut( 0xa0, n, temp );

        if( ( temp[ 0 ] == 7 ) && ( temp[ n-3 ] == 13 ) && ( temp[ n-1 ] == h ) ) {
            dataReadCorrect = true;
        } else {
            dataReadCorrect = false;

            break;
        }
    }

    if( dataReadCorrect ) {
        printf( "The data read are correct\n" );
    } else {
        printf( "The data read are incorrect\n" );
    }

    clock_t end = clock();

    printf( "Time elapsed: %0.2f ms", double( diffclock( end, begin ) ) );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }

    printf( "%s%s", "SetWireInValue", str );

    if( debug_en ) {
        getchar();
    } else {
        printf( "\n" );
    }
    xem->SetWireInValue( 0x00, 0x05, 0x03 );
}
//---------------------------------------------------------------------------------------------------------------------------------

template <class FrontPanel>
int OK_Main()
{
    int count;
    string strSerial;
    int result;

    FrontPanel *fptemp;
    FrontPanel *fp = new FrontPanel();

    printf( "\n%s\n","------- API OpenOK/FrontPannel Tester-------" );

    printf( "\nUsing API %s\n", fp->GetOpenOKVersion().c_str() );
    printf( "\nUsing API %s\n", fp->GetLibUSBVersion().c_str() );

    count = fp->GetDeviceCount();

    count = count > 0 ? count : 0;

    printf( "\n%s\n","--------------------------------------------" );
    
    printf( "\nFound %d board(s) Opal Kelly connected\n\n", count );
    
    printf( "%s\n","--------------------------------------------" );

    if( count > 0 ) {
        fptemp = new FrontPanel[ count ];

        for( int ct = 0; ct < count ; ct++ ) {
            strSerial = fp->GetDeviceListSerial( ct );

            printf( "%s%s","Open USB Port", str );

            if( debug_en ) {
                getchar();
            } else {
                printf( "\n" );
            }

            result = fptemp[ ct ].OpenBySerial( strSerial );

            if( debug_en ) {
                if( result >= 0 ) {
                    printf( "%s%s","Opened USB Port", str );
                    getchar();
                } else {
                    printf( "%s%s","Not Opened USB Port",str );
                    getchar();
                }
            }

            if( result >= 0 ) {
                OK_Test( &fptemp[ ct ] );

                printf( "\n%s\n","--------------------------------------------" );
            }
        }
        delete[] fptemp;
    }
    delete fp;
}
//---------------------------------------------------------------------------------------------------------------------------------

int main( int argc, char *argv[] )
{  
    OK_Main< OpenOK >();

    return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------
