using System;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using Math;

namespace Math
{
    /// <summary>
    /// A class describing polynomials in one indeterminate.  
    /// </summary>
    /// <remarks>
    /// Inherits from <see cref="System.Collections.Generic.IEnumerable&le;T&ge;"/> because it is a collection of coefficients.  
    /// </remarks>
    public class Polynomial : IEnumerable<Fraction>
    {
        //In honor of the fact that most polynomials are written in terms of x
        public static readonly Polynomial x = new Polynomial(0, 1);

        /// <summary>
        /// A property for reading the coefficients as an array of fractions.  
        /// </summary>
        /// <remarks>
        /// Note that only a get accessor is provided.  While individual coefficients may be modified, the array as a whole may not be resized.
        /// To shift coefficients left, use the left shift operator.  To shift coefficients right, use the right shift operator.
        /// Coefficients are released in order of increasing monomial exponent, i.e. <code>Coefficients[0]</code> corresponds to the 
        /// <code>x^0</code>th term, <code>Coefficients[1]</code> corresponds to <code>x^1</code>, ...
        /// </remarks>
        public Fraction[] Coefficients { get; }
        public Fraction LeadingCoeff
        {
            get
            {
                return Coefficients[LongDegree];
            }
            set
            {
                Coefficients[LongDegree] = value;
            }
        }
        public Fraction ConstTerm
        {
            get
            {
                return Coefficients[0];
            }
            set
            {
                Coefficients[0] = value;
            }
        }
        /// <summary>
        /// A constructor for a polynomial with predetermined coefficients.  
        /// </summary>
        /// <remarks>
        /// Coefficients should be provided in order of increasing monomial exponent, i.e. 
        /// <code>new Polynomial(1, 2, 3).ToString('x')</code> yields <code>"1 + 2x + 3x^2"</code>
        /// </remarks>
        /// <param name="coefficients">
        /// The coefficients the new polynomial will have; cannot be empty.  
        /// If you pass no parameters, Polynomial will throw ArgumentException
        /// </param>
        public Polynomial(params Fraction[] coefficients)
        {
            if (coefficients.LongLength == 0)
            {
                throw new ArgumentException("No parameters passed to create a new Polynomial.");
            }
            if (coefficients[coefficients.Length - 1] == 0)
            {
                Stack<Fraction> stack = new Stack<Fraction>(coefficients);
                do
                {
                    stack.Pop();
                }
                while (stack.Peek() == 0);
                coefficients = stack.ToArray();
            }
            Coefficients = coefficients;
        }
        /// <summary>
        /// Converts a polynomial to its string representation.  
        /// </summary>
        /// <param name="variable">
        /// A character used to denote this polynomial's indeterminate,
        /// i.e. if the polynomial object is considered a member of Q[x], pass 'x'; if Q[t], pass 't'.
        /// </param>
        /// <returns>
        /// A string representation of the polynomial.  
        /// </returns>
        public string ToString(char variable)
        {
            StringBuilder str = new StringBuilder(ConstTerm.ToString());
            if (Coefficients.LongLength > 1)
            {
                str.Append(" + ");
                long end = LongDegree;
                for (long index = 1; index < end; index++)
                {
                    str.AppendFormat("{0}{1}^{2} + ", Coefficients[index].ToString(), variable, index);
                }
                str.AppendFormat("{0}{1}^{2}", Coefficients[end].ToString(), variable, end);
            }
            return str.ToString();
        }
        /// <summary>
        /// Evaluates a univariate polynomial at a given value.  
        /// </summary>
        /// <remarks>
        /// The polynomial is evaluated using standard form, not nested form, despite additional overhead for 
        /// code simplicity.  
        /// </remarks>
        /// <param name="value">
        /// The x-value the univariate polynomial will be evaluated at.  
        /// </param>
        /// <returns>
        /// The value of the polynomial at the given x-value.  
        /// </returns>
        public Fraction Evaluate(Fraction value)
        {
            Fraction partial = 0;
            for (long index = 0; index < Coefficients.LongLength; index++)
            {
                partial += Coefficients[index] * (value ^ index);
            }
            return partial;
        }
        public IEnumerator<Fraction> GetEnumerator()
        {
            return ((IEnumerable<Fraction>)Coefficients).GetEnumerator();
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
        public Fraction this[long index]
        {
            get
            {
                return Coefficients[index];
            }
            set
            {
                Coefficients[index] = value;
            }
        }
        /// <summary>
        /// A property for the degree of the polynomial.  
        /// </summary>
        public int Degree
        {
            get
            {
                return Coefficients.Length - 1;
            }
        }
        public long LongDegree
        {
            get
            {
                return Coefficients.LongLength - 1;
            }
        }
        public static Polynomial operator +(Polynomial larg, Polynomial rarg)
        {
            if (larg.Coefficients.LongLength < rarg.Coefficients.LongLength)
            {
                Polynomial temp = larg;
                larg = rarg;
                rarg = temp;
            }
            Fraction[] coeffs = larg.Coefficients.Clone() as Fraction[];
            for (long index = 0; index < rarg.Coefficients.LongLength; index++)
            {
                coeffs[index] += rarg.Coefficients[index];
            }
            return new Polynomial(coeffs);
        }
        public static Polynomial operator -(Polynomial larg, Polynomial rarg)
        {
            if (larg.Coefficients.LongLength < rarg.Coefficients.LongLength)
            {
                Polynomial temp = larg;
                larg = rarg;
                rarg = temp;
            }
            Fraction[] coeffs = larg.Coefficients.Clone() as Fraction[];
            for (long index = 0; index < rarg.Coefficients.LongLength; index++)
            {
                coeffs[index] -= rarg.Coefficients[index];
            }
            return new Polynomial(coeffs);
        }
        public static Polynomial operator *(Polynomial larg, Fraction rarg)
        {
            Fraction[] coeffs = larg.Coefficients.Clone() as Fraction[];
            for (long index = 0; index < coeffs.LongLength; index++)
            {
                coeffs[index] *= rarg;
            }
            return new Polynomial(coeffs);
        }
        public static Polynomial operator /(Polynomial larg, Fraction rarg)
        {
            Fraction[] coeffs = larg.Coefficients.Clone() as Fraction[];
            for (long index = 0; index < coeffs.LongLength; index++)
            {
                coeffs[index] /= rarg;
            }
            return new Polynomial(coeffs);
        }
        public static Polynomial operator *(Fraction larg, Polynomial rarg)
        {
            return rarg * larg;
        }
        public static Polynomial operator *(Polynomial larg, Polynomial rarg)
        {
            Fraction[] coeffs = new Fraction[larg.Degree + rarg.Degree + 1];
            for (long index = 0; index < coeffs.LongLength; index++)
            {
                coeffs[index] = 0;
            }
            for (long index = 0; index < rarg.Coefficients.Length; index++)
            {
                for (long jndex = 0; jndex < rarg.Coefficients.Length; index++)
                {
                    coeffs[index + jndex] += larg.Coefficients[index] * rarg.Coefficients[jndex];
                }
            }
            return new Polynomial(coeffs);
        }
        public static Polynomial operator ^(Polynomial larg, ulong rarg)
        {
            Polynomial retval = (Fraction)1;
            while (rarg > 0)
            {
                retval *= larg;
                rarg--;
            }
            return retval;
        }
        public static Polynomial operator /(Polynomial larg, Polynomial rarg)
        {
            //Prevent division by zero, if possible
            if (rarg.LeadingCoeff == 0)
            {
                rarg = new Polynomial(rarg.Coefficients);
            }
            Fraction[] coeffs = new Fraction[larg.LongDegree - rarg.LongDegree + 1];
            Fraction start = rarg.LeadingCoeff;
            for (long index = coeffs.LongLength - 1, jndex = larg.LongDegree; index >= 0; index--, jndex--)
            {
                coeffs[index] = larg.Coefficients[jndex] / start;
                larg -= rarg * coeffs[index];
            }
            return new Polynomial(coeffs);
        }
        public static Polynomial operator %(Polynomial larg, Polynomial rarg)
        {
            //Prevent division by zero, if possible
            if (rarg.LeadingCoeff == 0)
            {
                rarg = new Polynomial(rarg.Coefficients);
            }
            Fraction start = rarg.LeadingCoeff;
            for (long index = larg.LongDegree, end = rarg.LongDegree; index >= end; index--)
            {
                larg -= rarg * (larg.Coefficients[index] / start);
            }
            return larg;
        }
        public static Polynomial operator >>(Polynomial larg, int rarg)
        {
            if (rarg < 0)
            {
                return larg << -rarg;
            }
            Fraction[] coeffs = new Fraction[larg.Coefficients.LongLength + rarg];
            larg.Coefficients.CopyTo(coeffs, rarg);
            for (rarg--; rarg >= 0; rarg--)
            {
                coeffs[rarg] = 0;
            }
            return new Polynomial(coeffs);
        }
        public static Polynomial operator <<(Polynomial larg, int rarg)
        {
            if (rarg < 0)
            {
                return larg >> -rarg;
            }
            Fraction[] coeffs = new Fraction[larg.Coefficients.LongLength - rarg];
            for (long index = coeffs.LongLength - 1, jndex = larg.LongDegree; index >= 0; index--, jndex--)
            {
                coeffs[index] = larg[index];
            }
            return new Polynomial(coeffs);
        }
        public static implicit operator Polynomial(Fraction arg)
        {
            return new Polynomial(arg);
        }
    }
}