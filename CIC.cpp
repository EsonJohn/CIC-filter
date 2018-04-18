/* (C) 2016 EsonJohn(zhangyibupt@163.com) */

#include "CIC.h"

#include <assert.h>
#include <memory.h>

CIC::CIC(int decimationFactor, int numberOfSections, int diferrencialDelay):
	R(decimationFactor),
	N(numberOfSections),
	M(diferrencialDelay)
{
	this->buffer_comb = NULL;
	this->buffer_integrator = NULL;
	this->offset_comb = NULL;

	assert(R > 0 && N > 0 && M > 0);

	this->buffer_integrator = new double[N];
	this->offset_comb = new int[N];
	this->buffer_comb = new double*[N];
	for (int i = 0; i < N; i++)
		buffer_comb[i] = new double[M];
}

CIC::~CIC()
{
	// release buffer
	delete this->buffer_integrator;
	delete this->offset_comb;
	for (int i = 0; i < this->N; i++)
		delete this->buffer_comb[i];
	delete this->buffer_comb;
}

void CIC::reset()
{
	// clear buffer
	for (int i = 0; i < N; i++)
	{
		this->buffer_integrator[i] = 0;
		this->offset_comb[i] = 0;
		for (int j = 0; j < M; j++)
			this->buffer_comb[i][j] = 0;
	}
}

double CIC::filter(double *input, int length)
{
	if (length != this->R)
		return 0;

	double anttenuation = 1.0;	// the amplitude anttenuation factor, default as 1
	double tmp_out = 0;
	
	// Integrator part
	for (int i = 0; i < R; i++)
	{
		tmp_out = input[i];

		for (int j = 0; j < N; j++)
			tmp_out = this->buffer_integrator[j] = this->buffer_integrator[j] + tmp_out;
	}

	// Comb part
	for (int i = 0; i < N; i++)
	{
		this->offset_comb[i] = (this->offset_comb[i] + 1) % M;
		double tmp = this->buffer_comb[i][this->offset_comb[i]];
		this->buffer_comb[i][this->offset_comb[i]] = tmp_out;
		tmp_out = tmp_out - tmp;
	}

	return anttenuation * tmp_out;
}