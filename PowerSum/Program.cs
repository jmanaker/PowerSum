using System;
using System.Linq;
using Math;
using System.Text;

namespace PowerSum
{
    class Program
    {
        static void Main(string[] args)
        {
            ulong power;
            int point;
            Polynomial poly = new Polynomial(new Fraction[3]{
                .5, .5, 0
            });
            Console.Write("What power should PowerSum sum?");
            power = ulong.Parse(Console.ReadLine());
            Console.Write("What should the formula be evaluated at?");
            point = int.Parse(Console.ReadLine());
            Console.WriteLine("Working...Please Wait");
            if (power >= 1)
            {
                for (int counter = 1; (ulong)counter < power; counter++)
                {
                    PolyInt(poly);
                    Fraction temp = 1;
                    foreach (Fraction coefficient in poly.Coefficients)
                    {
                        temp -= coefficient;
                    }
                    poly[0] = temp;
                    GC.Collect();
                }
            }
            Fraction eval = poly.Evaluate(point);
            Console.WriteLine("Formula: " + poly.ToString());
            Console.WriteLine("Evaluation: " + eval.ToString() + " ≈ " + ((double)eval).ToString());
            Console.ReadKey();
        }
        static Polynomial PolyInt(Polynomial p)
        {
            p.degree++;
            for (int index = p.degree - 1; index >= 0; index--)
            {
                p[index + 1] = p[index] / (index + 1);
            }
            p[0] = 1 - p.Coefficients.Aggregate<Fraction>(new Func<Fraction,Fraction,Fraction>(delegate(
                Fraction first, Fraction second)
                {
                    return first + second;
                }));
            return p;
        }
    }
}
