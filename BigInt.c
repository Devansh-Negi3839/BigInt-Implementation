#include <stdio.h>
#include <string.h>

typedef struct BigInt_Tag
{
    int arr[1000];
    int size;
} BigInt;

BigInt BigIntInput()
{
    printf("Enter Number:->\t");
    char str[1000];
    scanf("%s", str);
    int length = strlen(str);
    BigInt arr;
    int idx = 0;
    for (int i = length - 1; i >= 0; i--)
        arr.arr[idx++] = str[i] - '0';
    arr.size = idx;
    return arr;
}

BigInt BigIntfromInt(int n)
{
    BigInt result;
    int t = n, length = 0, idx = 0;
    while (t > 0)
    {
        result.arr[idx++] = t % 10;
        t /= 10;
    }
    result.size = idx;
    return result;
}

void printBigInt(BigInt n)
{
    for (int i = n.size - 1; i >= 0; i--)
        printf("%d", n.arr[i]);

    printf("\n");
}

BigInt BigIntsubtract(BigInt a, BigInt b)
{
    BigInt ans;
    if (a.size < b.size)
        return BigIntsubtract(b, a);

    else if (a.size == b.size)
    {
        int i = a.size - 1;
        while (i >= 0 && a.arr[i] == b.arr[i])
        {
            i--;
        }
        if (i == -1) // a=b case
        {
            ans.arr[0] = 0;
            ans.size = 1;
            return ans;
        }
        if (a.arr[i] < b.arr[i])
        {
            return BigIntsubtract(b, a);
        }
    }

    int idx = 0;
    int j = 0;
    for (int i = 0; i < b.size; i++)
    {
        if (a.arr[i] - b.arr[i] < 0)
        {
            a.arr[i + 1]--;
            a.arr[i] += 10;
        }
        ans.arr[idx] = a.arr[i] - b.arr[i];
        if (ans.arr[idx] != 0)
        {
            j = i;
        }
        idx++;
    }

    for (int i = b.size; i < a.size; i++)
    {
        if (a.arr[i] != 0)
        {
            j = i;
        }
        if (a.arr[i] < 0)
        {
            a.arr[i + 1]--;
            a.arr[i] += 10;
        }
        ans.arr[idx] = a.arr[i];
        idx++;
    }
    ans.size = j + 1;
    return ans;
}

BigInt BigIntadd(BigInt a, BigInt b)
{
    if (a.size < b.size)
    {
        return BigIntadd(b, a);
    }
    BigInt ans;
    int idx = 0, carry = 0;
    for (int i = 0; i < b.size; i++)
    {
        int temp = a.arr[i] + b.arr[i] + carry;
        ans.arr[idx] = temp % 10;
        carry = temp / 10;
        idx++;
    }
    for (int i = b.size; i < a.size; i++)
    {
        int temp = a.arr[i] + carry;
        ans.arr[idx] = temp % 10;
        carry = temp / 10;
        idx++;
    }
    if (carry > 0)
    {
        ans.arr[idx] = carry;
        idx++;
    }
    ans.size = idx;
    return ans;
}

BigInt BigIntmultiply(BigInt a, BigInt b)
{
    BigInt ans;
    ans.size = 0;
    for (int i = 0; i < a.size; i++)
    {
        int idx = 0, carry = 0;
        BigInt t;
        for (int j = 0; j < b.size; j++)
        {
            int temp = a.arr[i] * b.arr[j] + carry;
            t.arr[idx] = temp % 10;
            carry = temp / 10;
            idx++;
        }
        if (carry > 0)
        {
            t.arr[idx] = carry;
            idx++;
        }
        t.size = idx;
        BigInt prod;
        int ind = 0;
        for (int p = 0; p < i; p++) // to add zero for next partial product
        {
            prod.arr[ind] = 0;
            ind++;
        }

        for (int p = 0; p < t.size; p++)
        {
            prod.arr[ind] = t.arr[p];
            ind++;
        }
        prod.size = ind;
        ans = BigIntadd(ans, prod);
    }
    return ans;
}

BigInt BigIntmodulo(BigInt a, BigInt b)
{
    while (a.size >= b.size)
    {
        if (a.size == b.size)
        {
            int i = a.size - 1;
            while (i >= 0 && a.arr[i] == b.arr[i])
            {
                i--;
            }
            if (i == -1)
            {
                a.arr[0] = 0;
                a.size = 1;
                break;
            }
            if (a.arr[i] < b.arr[i])
            {
                break;
            }
        }
        a = BigIntsubtract(a, b);
    }
    return a;
}

BigInt BigIntFactorial(BigInt a)
{
    BigInt ans;
    ans.arr[0] = 1;
    ans.size = 1;
    BigInt dcr;
    dcr.arr[0] = 1;
    dcr.size = 1;
    while (a.size != 1 || a.arr[0] != 0)
    {
        ans = BigIntmultiply(ans, a);

        a = BigIntsubtract(a, dcr);
    }
    return ans;
}

BigInt BigIntPowerInt(BigInt a, int n)
{
    if (n > 0)
    {
        BigInt ans;
        ans.arr[0] = 1;
        ans.size = 1;
        while (n > 0)
        {
            ans = BigIntmultiply(ans, a);
            n--;
        }
        return ans;
    }
}

// 1 if a>b, 0 a=b ,-1 a<b
int BigIntCompare(BigInt a, BigInt b)
{
    if (a.size < b.size)
        return -1;
    else if (a.size > b.size)
        return 1;
    else
    {
        for (int i = 0; i < a.size; i++)
        {
            if (a.arr[i] > b.arr[i])
                return 1;
            else if (a.arr[i] < b.arr[i])
                return -1;
        }
        return 0;
    }
}

BigInt reversestring(BigInt n)
{
    int size = n.size;
    for (int i = 0; i <= size / 2; i++)
    {
        int t = n.arr[i];
        n.arr[i] = n.arr[size - i - 1];
        n.arr[size - i - 1] = t;
    }

    return n;
}

BigInt BigIntDivide(BigInt x, long divisor)
{
    BigInt quotient;
    quotient.arr[0] = 0;
    quotient.size = 1;

    long temp = 0;
    int i = x.size - 1, j = 0;
    // printf("In func\n");

    for (int i = x.size - 1; i >= 0; i--)
    {
        temp = temp * 10 + x.arr[i];
        if (temp < divisor)
        {
            quotient.arr[j++] = 0;
        }
        else
        {
            quotient.arr[j++] = temp / divisor;
            temp = temp % divisor;
        }
    }
    quotient.size = j;
    // printBigInt(x);
    // printBigInt(quotient);
    // quotient.arr[j] = '\0';

    quotient = reversestring(quotient);
    // printBigInt(quotient);

    // trim extra zeroes
    int start = quotient.size - 1;
    // printf("\nstart is at %d with value %d\n", start, quotient.arr[start]);
    while (start >= 0 && quotient.arr[start] == 0)
    {
        start--;
        // printf("In LOOP\n");
    }
    if (start == -1)
    {
        quotient.size = 1;
        quotient.arr[0] = 0;
    }
    else
    {
        // printf("\nvalue of start is %d\n", start);
        // int k = quotient.size - 1;
        // while (start >= 0)
        // {
        //     quotient.arr[k--] = quotient.arr[start--];
        // }
        quotient.size = start + 1;
    }

    return quotient;
}

BigInt op_on_start(BigInt start, BigInt n)
{
    BigInt two, hundred;
    // tw0.arr[1] = 1;
    two.arr[0] = 2;
    two.size = 1;
    hundred.arr[2] = 1;
    hundred.arr[1] = 0;
    hundred.arr[0] = 0;
    hundred.size = 3;
    while (BigIntCompare(BigIntmultiply(hundred, BigIntmultiply(start, start)), n) == -1)
    {
        start = BigIntmultiply(start, two);
    }
    return start;
}
BigInt op_on_end(BigInt end, BigInt n)
{
    BigInt hundred, ten;
    ten.arr[1] = 1;
    ten.arr[0] = 0;
    ten.size = 2;
    hundred.arr[2] = 1;
    hundred.arr[1] = 0;
    hundred.arr[0] = 0;
    hundred.size = 3;
    while (BigIntCompare(BigIntmultiply(end, end), BigIntmultiply(n, hundred)) == 1)
    {
        end = BigIntDivide(end, 2);
    }
    return end;
}

BigInt BigIntSqrtInt(BigInt n)
{

    BigInt start, end, mid, one;

    start.arr[0] = 1;
    start.size = 1;
    one.arr[0] = 1;
    one.size = 1;
    // printBigInt(end);

    end.size = n.size;
    for (int i = n.size - 1; i >= 0; i--)
        end.arr[i] = n.arr[i];

    end = BigIntadd(end, one);

    // printf("\nstart,end is\n");
    start = op_on_start(start, n);
    // printBigInt(start);
    end = op_on_end(end, n);
    // printBigInt(end);
    int i = 0;
    // && BigIntCompare(start, end) <= 0
    while (i < 1000)
    {
        i++;
        mid = BigIntDivide(BigIntadd(start, end), 2);
        // printf("\nstart,mid end is\n");
        // printBigInt(start);
        // printBigInt(mid);
        // printBigInt(end);

        if (BigIntCompare(BigIntmultiply(mid, mid), n) == 0)
            return mid;
        else if (BigIntCompare(BigIntmultiply(mid, mid), n) == -1) // mid*mid <n
            start = BigIntadd(mid, one);
        else
            end = BigIntsubtract(mid, one);
    }
    return start;
}

int main()
{

    printf("\na)Modulo\n");
    BigInt first = BigIntInput();
    BigInt second = BigIntInput();
    first = BigIntmodulo(first, second);
    printBigInt(first);

    //

    printf("\nb)big int sqrt\n");
    BigInt q2 = BigIntInput();
    q2 = BigIntSqrtInt(q2);
    printBigInt(q2);

    //

    printf("\nc)big int power\n");
    BigInt q3 = BigIntfromInt(100);
    q3 = BigIntPowerInt(q3, 200);
    printBigInt(q3);

    //

    printf("\nd)Int to Big Int\n");
    BigInt q4 = BigIntfromInt(123456);
    printBigInt(q4);

    //

    printf("\ne)Factorial\n");
    BigInt q5 = BigIntfromInt(100);
    q5 = BigIntFactorial(q5);
    printBigInt(q5);

    return 0;
}