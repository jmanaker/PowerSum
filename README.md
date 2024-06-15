# PowerSum
As is well-known, 
> 1+1+1+... (n summands)=n
> 1+2+3+... (n summmands)=n(n+1)/2
> 1+4+9+... (n summands)=n(n+1)(2n+1)/6

Call the respective right-hand sides f1(n), f2(n), and f3(n).  What is fk(n)?  

One trick is to apply linearity: if p(j)=aj^2+bj+c, then
> p(1)+p(2)+p(3)+... (n summands)=af3(n)+bf2(n)+cf1(n).

In particular, if we know f1 and f2, but not f3, and we also know a convenient choice of p to deduce the right-hand side, then by subtracting off bf2(n)+cf1(n) and dividing by a, we can solve for f3.  More generally, given such a p of degree k-1 and all f_j for j<k, we can deduce f_k.  If we know a family of such p for each k, we can deduce all f_j by recursion.

> n^k = (n^k - (n-1)^k) + ((n-1)^k - (n-2)^k) + ... + (1^k - 0)

n^k-(n-1)^k has degree k-1.  

This is a C# program to recursively compute such coefficients.  It is very old (dating to my middle-school years) and likely extremely inefficient; you're probably better off rolling your own.  
