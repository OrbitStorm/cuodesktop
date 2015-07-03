#include "stdafx.h"

#include "EncPatcher.h"
#include "LoginEncryption.h"

#define N2L(C, LL) \
    LL  = ((unsigned int)(*((C)++))) << 24, \
    LL |= ((unsigned int)(*((C)++))) << 16, \
    LL |= ((unsigned int)(*((C)++))) << 8, \
    LL |= ((unsigned int)(*((C)++)))

DWORD zero = 0;

const DWORD *LoginEncryption::Key1 = &zero;
const DWORD *LoginEncryption::Key2 = &zero;

LoginEncryption::LoginEncryption()
{
	m_Table[0] = m_Table[1] = 0;
}

void LoginEncryption::SetKeys( const DWORD *k1, const DWORD *k2 )
{
	//LoginEncryption::StaticKey1  = *((DWORD*)sk1);
	LoginEncryption::Key1 = k1;
	LoginEncryption::Key2 = k2;
	//MessageBox( NULL, "jeff is a tool", "blah", 0 );
}

void LoginEncryption::Initialize( const BYTE *pSeed )
{
	DWORD seed;
    N2L(pSeed, seed);

    m_Table[0] = (((~seed) ^ 0x00001357) << 16) | ((seed ^ 0xffffaaaa) & 0x0000ffff);
    m_Table[1] = ((seed ^ 0x43210000) >> 16) | (((~seed) ^ 0xabcdffff) & 0xffff0000);
}

void LoginEncryption::Encrypt( const BYTE *in, BYTE *out, int len )
{
	for (int i=0;i<len;i++)
		out[i] = Crypt( in[i] );
}

void LoginEncryption::Decrypt( const BYTE *in, BYTE *out, int len )
{
	for (int i=0;i<len;i++)
		out[i] = Crypt( in[i] );
}

BYTE LoginEncryption::Crypt( BYTE in )
{
	BYTE out = in ^ ((unsigned char)m_Table[0]);
	
	DWORD OldT1 = m_Table[1];
	m_Table[1] = ((((m_Table[1] >> 1) | (m_Table[0] << 31)) ^ ((*Key1)-1)) >> 1 | (m_Table[0] << 31)) ^ (*Key1);
	m_Table[0] = ((m_Table[0]>>1) | (OldT1<<31)) ^ (*Key2);
	
	return out;
}

BYTE LoginEncryption::Test( BYTE in )
{
	return in ^ ((unsigned char)m_Table[0]);
}

/*DWORD LoginEncryption::GenerateBadSeed( DWORD oldSeed )
{	
	//DWORD newSeed = ((*Key1)+1) ^ ((*Key2)); //This one doesnt say encrypted but hangs 
	//DWORD newSeed = ((*Key1)) ^ ((*Key2)+1); //Encrypted
	//DWORD newSeed = ((*Key1)-1) ^ ((*Key2-1)); //Encrypted
	//DWORD newSeed = ((*Key1)-1) ^ ((*Key2)); //Encrypted
	//DWORD newSeed = (*Key1) ^ (*Key2) ^ 1; //Encrypted
	//DWORD newSeed = ((*Key1) ^ (*Key2)) + 1; //Encrypted
	//DWORD newSeed = ((*Key1)+1) ^ ((*Key2)-1); //Encrypted
	DWORD newSeed = ((*Key1)+1) ^ ((*Key2)); 
	newSeed = ((newSeed >> 24) & 0xFF) | ((newSeed >> 8) & 0xFF00) | ((newSeed << 8) & 0xFF0000) | ((newSeed << 24) & 0xFF000000);
	return htonl( newSeed ^ htonl( oldSeed ) );
}*/

DWORD LoginEncryption::GenerateBadSeed( DWORD oldSeed )
{
	DWORD newSeed = (*Key1) ^ (*Key2);
	newSeed = ((newSeed >> 24) & 0xFF) | ((newSeed >> 8) & 0xFF00) | ((newSeed << 8) & 0xFF0000) | ((newSeed << 24) & 0xFF000000);
	DWORD reallyNewSeed = htonl( newSeed ^ htonl( oldSeed ) );
	//char temp[256];
	//sprintf( temp, "OldSeed = %08X\nKey1 = %08X, Key2 = %08X\nNewSeed = %08X\nReallyNewSeed = %08X", oldSeed, *Key1, *Key2, newSeed, reallyNewSeed );
	//MessageBox( NULL, temp, "I hate EA", MB_OK );
	return reallyNewSeed;
}

