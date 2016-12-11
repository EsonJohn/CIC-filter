/* (C) 2016 EsonJohn(zhangyibupt@163.com) */

#ifndef CIC_H
#define CIC_H

#include <stdio.h>
#include <cmath>

class CIC
{
public:
	// R:Decimation factor;
	// N:Number of sections;
	// M:Differential delay;
	CIC(int decimationFactor, int numberOfSections, int diferrencialDelay);

	// destructor
	~CIC();

	// the actual filter function(input shuld be R continues samples and length should be R)
	double filter(double *input, int length);

	// reset the buffer
	void reset();

private:
	int			R, N, M;
	double		*buffer_integrator;	// buffer of the integrator part
	double		**buffer_comb;		// buffer of the comb part
	int			*offset_comb;

};

#endif
