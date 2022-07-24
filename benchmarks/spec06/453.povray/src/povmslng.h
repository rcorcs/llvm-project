#ifndef POVMS_SIMPLE_LONG_H
#define POVMS_SIMPLE_LONG_H

class POVMS_Simple_Long
{
	public:
		int value[2];

		POVMS_Simple_Long()
		{
			value[0] = 0;
			value[1] = 0;
		}

		POVMS_Simple_Long(int hi, unsigned int lo)
		{
			value[0] = (int)lo;
			value[1] = (int)hi;
		}

		POVMS_Simple_Long(unsigned long l)
		{
			value[0] = (int)l;
			value[1] = 0;
		}

		POVMS_Simple_Long(long l)
		{
			value[0] = (int)l;
			value[1] = 0;
		}

		POVMS_Simple_Long(int l)
		{
			value[0] = l;
			value[1] = 0;
		}

		POVMS_Simple_Long(double l)
		{
			value[0] = (int)l;
			value[1] = 0;
		}

		POVMS_Simple_Long(float l)
		{
			value[0] = (int)l;
			value[1] = 0;
		}

		POVMS_Simple_Long *operator&()
		{
			return (POVMS_Simple_Long *)value;
		}

		operator unsigned long() const
		{
			return (unsigned long)(value[0]);
		}

		operator long() const
		{
			return (long)(value[0]);
		}

		operator int() const
		{
			return (int)(value[0]);
		}

		operator double() const
		{
			return (double)(value[0]);
		}

		operator float() const
		{
			return (float)(value[0]);
		}

		bool operator>(int l) const
		{
			return (value[0] > l);
		}

		bool operator>=(int l) const
		{
			return (value[0] >= l);
		}

		bool operator<(int l) const
		{
			return (value[0] < l);
		}

		bool operator<=(int l) const
		{
			return (value[0] < l);
		}

		bool operator==(int l) const
		{
			return (value[0] == l);
		}

		bool operator!=(int l) const
		{
			return (value[0] != l);
		}
};

#define POVMSLong POVMS_Simple_Long

#define SetPOVMSLong(v,h,l) *v = POVMSLong((int)(h), (unsigned int)(l));
#define GetPOVMSLong(h,l,v) *l = int(v); h = 0

#define POVMSLongToCDouble(x) double(x)

#endif
