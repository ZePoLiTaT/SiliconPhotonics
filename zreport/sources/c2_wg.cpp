double Line::GetLatency(int indexIn, int indexOut)
{
    return Length_Line*LatencyRate_Line;
}

double Line::GetPropagationLoss(int indexIn, int indexOut, double wavelength)
{
    if(indexIn != indexOut)
    {
	return Length_Line*PropagationLoss;
    }
    else
    {
	return MAX_INSERTION_LOSS;
    }
}

void Line::SetRoutingTable()
{
    routingTable[0] = 1;
    routingTable[1] = 0;
}

