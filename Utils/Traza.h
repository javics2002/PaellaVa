#pragma once

#define TRACE_F(F, L, T) (cout << F <<":" << L << " "<<#T << ": " << T << endl )
#define TRACE( T) (cout << #T << ": " << T << endl )