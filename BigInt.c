#include <stdio.h>
#include <string.h>

typedef struct BigInt_Tag
{
    int arr[1000];
    int size;
    int sign; // 0 for positive, 1 for negative
} BigInt;

// Function to input BigInt
BigInt BigIntInput()
{
    printf("Enter Number:->\t");
    char str[1000];
    scanf("%s", str);
    int length = strlen(str);
    BigInt arr;
    int idx = 0, end = 0;
    if (str[0] == '-')
    {
        arr.sign = 1;
        end = 1;
    }
    else
        arr.sign = 0;
    for (int i = length - 1; i >= end; i--)
        arr.arr[idx++] = str[i] - '0';
    arr.size = idx;
    return arr;
}

// Function to convert int to BigInt
BigInt BigIntfromInt(int n)
{
    BigInt result;

    // Checking for negative sign
    if (n > 0)
        result.sign = 0;
    else
    {
        result.sign = 1;
        n = -n;
    }

    int t = n, length = 0, idx = 0;
    while (t > 0)
    {
        result.arr[idx++] = t % 10;
        t /= 10;
    }
    result.size = idx;
    return result;
}

// Function to print BigInt
void printBigInt(BigInt n)
{
    if (n.sign == 1)
        printf("-");
    for (int i = n.size - 1; i >= 0; i--)
        printf("%d", n.arr[i]);

    printf("\n");
}

// Function to subtract two BigInts in O(N) where N is size of the number
BigInt BigIntsubtract(BigInt a, BigInt b)
{
    BigInt ans;

    // Ensure a is the larger number
    if (a.size < b.size)
        return BigIntsubtract(b, a);

    // If sizes are equal, compare the digits to determine the larger number
    else if (a.size == b.size)
    {
        int i = a.size - 1;
        while (i >= 0 && a.arr[i] == b.arr[i])
        {
            i--;
        }
        if (i == -1) // a == b case
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

    // Subtract corresponding digits of a and b
    for (int i = 0; i < b.size; i++)
    {
        if (a.arr[i] - b.arr[i] < 0)
        {
            a.arr[i + 1]--; // Borrow from the next digit
            a.arr[i] += 10;
        }
        ans.arr[idx] = a.arr[i] - b.arr[i];
        if (ans.arr[idx] != 0)
        {
            j = i;
        }
        idx++;
    }

    // Subtract remaining digits of a
    for (int i = b.size; i < a.size; i++)
    {
        if (a.arr[i] != 0)
        {
            j = i;
        }
        if (a.arr[i] < 0)
        {
            a.arr[i + 1]--; // Borrow from the next digit
            a.arr[i] += 10;
        }
        ans.arr[idx] = a.arr[i];
        idx++;
    }

    ans.size = j + 1;  // Set the size of the result
    ans.sign = a.sign; // Set the sign of the result
    return ans;
}

// Function to add two BigInts in O(N) where N is size of the number
BigInt BigIntadd(BigInt a, BigInt b)
{
    if (a.size < b.size) // Ensure a is the larger number
    {
        return BigIntadd(b, a);
    }

    BigInt ans;
    ans.sign = a.sign; // Set the sign of the result to the sign of a
    int idx = 0, carry = 0;
    // Add corresponding digits of a and b
    for (int i = 0; i < b.size; i++)
    {
        int temp = a.arr[i] + b.arr[i] + carry;
        ans.arr[idx] = temp % 10; // Store the unit place digit
        carry = temp / 10;        // Update the carry
        idx++;
    }
    // Add remaining digits of a with the carry
    for (int i = b.size; i < a.size; i++)
    {
        int temp = a.arr[i] + carry;
        ans.arr[idx] = temp % 10; // Store the unit place digit
        carry = temp / 10;        // Update the carry
        idx++;
    }
    // If there's a carry left, add it as the most significant digit
    if (carry > 0)
    {
        ans.arr[idx] = carry;
        idx++;
    }

    ans.size = idx; // Set the size of the result
    return ans;
}

// Function to multiply two BigInts in O(M*N) where M,N is size of the number
BigInt BigIntmultiply(BigInt a, BigInt b)
{
    BigInt ans;
    ans.size = 0; // Initialize result size to 0

    // Iterate over each digit of a
    for (int i = 0; i < a.size; i++)
    {
        int idx = 0, carry = 0;
        BigInt t;

        // Multiply the current digit of a with each digit of b
        for (int j = 0; j < b.size; j++)
        {
            int temp = a.arr[i] * b.arr[j] + carry;
            t.arr[idx] = temp % 10; // Store the unit place digit
            carry = temp / 10;      // Update the carry
            idx++;
        }
        // If there's a carry left, add it to the result
        if (carry > 0)
        {
            t.arr[idx] = carry;
            idx++;
        }
        t.size = idx; // Set the size of the partial product

        BigInt prod;
        int ind = 0;
        // Add leading zeros for the next partial product
        for (int p = 0; p < i; p++)
        {
            prod.arr[ind] = 0;
            ind++;
        }
        // Append the partial product digits
        for (int p = 0; p < t.size; p++)
        {
            prod.arr[ind] = t.arr[p];
            ind++;
        }
        prod.size = ind; // Set the size of the partial product
        // Add the partial product to the final result
        ans = BigIntadd(ans, prod);
    }

    ans.sign = a.sign ^ b.sign; // Determine the sign of the result
    return ans;
}

// Function to find modulo of two BigInt
BigInt BigIntmodulo(BigInt a, BigInt b)
{
    // Check if either number is negative
    if (a.sign == 1 || b.sign == 1)
    {
        printf("Modulo of negative number is not available\n");
        return a; // Return a as is if either number is negative
    }

    // Loop until the size of a is less than the size of b
    while (a.size >= b.size)
    {
        // If the sizes of a and b are equal, compare the numbers element by element
        if (a.size == b.size)
        {
            int i = a.size - 1;
            // Compare each element from the most significant to the least significant
            while (i >= 0 && a.arr[i] == b.arr[i])
            {
                i--; // Move to the next element if they are equal
            }

            // If all elements are equal, the result is zero
            if (i == -1)
            {
                a.arr[0] = 0;
                a.size = 1; // Set the result to zero with size 1
                break;
            }

            // If the element in a is smaller than in b, break out of the loop
            if (a.arr[i] < b.arr[i])
            {
                break;
            }
        }

        // Subtract b from a using the BigIntsubtract function
        a = BigIntsubtract(a, b);
    }

    // Return the result, which is the remainder
    return a;
}

// Function to find the factorial of a BigInt
BigInt BigIntFactorial(BigInt a)
{
    // Check if the number is negative
    if (a.sign == 1)
    {
        printf("Factorial of negative number is not defined\n");
        return a; // Return a as is if the number is negative
    }
    // Initialize the result (ans) to 1
    BigInt ans;
    ans.arr[0] = 1;
    ans.size = 1;

    // Initialize the decrement value (dcr) to 1
    BigInt dcr;
    dcr.arr[0] = 1;
    dcr.size = 1;
    // Loop until a becomes zero
    while (a.size != 1 || a.arr[0] != 0)
    {
        ans = BigIntmultiply(ans, a); // Multiply ans by the current value of a
        a = BigIntsubtract(a, dcr);   // Decrement a by 1
    }
    return ans;
}

// Function to find power of BigInt to integer
BigInt BigIntPowerInt(BigInt a, int n)
{
    int sign;
    if (a.sign == 1)
    {
        sign = n % 2 == 0 ? 0 : 1;
    }
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
        ans.sign = sign;
        return ans;
    }
}

// 1 if a>b, 0 a=b ,-1 a<b checks magnitude only
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

// Function to reverse BigInt
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

// Function to divide a BigInt by a long integer
BigInt BigIntDivide(BigInt x, long divisor)
{
    BigInt quotient;     // Initialize the quotient BigInt
    quotient.arr[0] = 0; // Start with quotient value 0
    quotient.size = 1;   // Initial size is 1

    // Determine the sign of the result
    int sign = x.sign ^ (divisor < 0);          // Result is negative if signs of x and divisor differ
    divisor = divisor < 0 ? -divisor : divisor; // Make divisor positive for calculation

    long temp = 0; // Temporary variable to hold intermediate results

    // Loop through each digit of x from most significant to least significant
    for (int i = x.size - 1; i >= 0; i--)
    {
        temp = temp * 10 + x.arr[i];                    // Shift temp left and add current digit of x
        quotient.arr[quotient.size++] = temp / divisor; // Append quotient digit
        temp = temp % divisor;                          // Update temp to the remainder
    }

    quotient = reversestring(quotient); // Reverse the quotient to correct order

    // Trim leading zeroes in the quotient
    while (quotient.size > 1 && quotient.arr[quotient.size - 1] == 0)
        quotient.size--;

    quotient.sign = sign; // Set the sign of the quotient
    return quotient;      // Return the resulting quotient
}

BigInt op_on_start(BigInt start, BigInt n)
{
    BigInt two, hundred;
    two.arr[0] = 2;
    two.size = 1;
    hundred.arr[2] = 1;
    hundred.arr[1] = 0;
    hundred.arr[0] = 0;
    hundred.size = 3;
    while (BigIntCompare(BigIntmultiply(hundred, BigIntmultiply(start, start)), n) == -1)
        start = BigIntmultiply(start, two);

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
        end = BigIntDivide(end, 2);

    return end;
}

// Function to find the square root of a BigInt
BigInt BigIntSqrtInt(BigInt n)
{
    // Check if the number is negative
    if (n.sign == 1)
    {
        printf("Square root of negative number is not defined\n");
        return n; // Return n as is if the number is negative
    }

    BigInt start, end, mid, one;

    // Initialize start and one to 1
    start.arr[0] = 1;
    start.size = 1;
    one.arr[0] = 1;
    one.size = 1;

    // Initialize end to n
    end.size = n.size;
    for (int i = n.size - 1; i >= 0; i--)
        end.arr[i] = n.arr[i];

    // Increment end by 1
    end = BigIntadd(end, one);

    // used to approximate values os start and end for start and end. start^2 * 100 < N
    start = op_on_start(start, n);
    end = op_on_end(end, n);

    int i = 0; // Iteration counter

    // Perform binary search to find the square root
    while (i < 1000) // Limit iterations to prevent infinite loop
    {
        i++;
        mid = BigIntDivide(BigIntadd(start, end), 2);
        if (BigIntCompare(BigIntmultiply(mid, mid), n) == 0)
            return mid;                                            // If mid*mid == n, mid is the square root
        else if (BigIntCompare(BigIntmultiply(mid, mid), n) == -1) // mid*mid < n
            start = BigIntadd(mid, one);                           // Move start to mid + 1
        else
            end = BigIntsubtract(mid, one); // Move end to mid - 1
    }

    return start; // Return the approximated square root
}

int main()
{
    int choice;
    printf("\n\n\nEnter choice:->\n 1)Add 2 BigInt(positive only) \n 2)Subtract 2 BigInt(positive only)\n 3)Multiply 2 BigInt\n 4)Divide BigInt by long\n 5)Modulo of 2 BigInt(Positive Only)\n 6)Power of BigInt to positive integer\n 7)Factorial of BigInt\n 8)Sqrt of BigInt\n 0)Exit Program\n\n\n");

    scanf("%d", &choice);
    while (choice != 0)
    {
        switch (choice)
        {
        case 1:
        {
            BigInt first = BigIntInput();
            BigInt second = BigIntInput();
            if (first.sign == 1 || second.sign == 1)
            {
                printf("Addition of negative number is not available\n");
            }
            else
            {
                BigInt ans = BigIntadd(first, second);
                printBigInt(ans);
            }
            break;
        }
        case 2:
        {
            BigInt first = BigIntInput();
            BigInt second = BigIntInput();
            BigInt ans = BigIntsubtract(first, second);
            printBigInt(ans);
            break;
        }
        case 3:
        {
            BigInt first = BigIntInput();
            BigInt second = BigIntInput();
            BigInt ans = BigIntmultiply(first, second);
            printBigInt(ans);
            break;
        }
        case 4:
        {
            BigInt first = BigIntInput();
            int second;
            scanf("%d", &second);
            BigInt ans = BigIntDivide(first, second);
            printBigInt(ans);
            break;
        }
        case 5:
        {
            BigInt first = BigIntInput();
            BigInt second = BigIntInput();
            if (first.sign == 1 || second.sign == 1)
            {
                printf("Modulo of negative number is not available\n");
            }
            else
            {
                BigInt ans = BigIntmodulo(first, second);
                printBigInt(ans);
            }
            break;
        }
        case 6:
        {
            BigInt first = BigIntInput();
            int second;
            scanf("%d", &second);
            if (second < 0)
            {
                printf("Power of negative number is not available\n");
            }
            else
            {
                BigInt ans = BigIntPowerInt(first, second);
                printBigInt(ans);
            }
            break;
        }
        case 7:
        {
            BigInt first = BigIntInput();
            if (first.sign == 1)
            {
                printf("Factorial of negative number is not defined\n");
            }
            else
            {
                BigInt ans = BigIntFactorial(first);
                printBigInt(ans);
            }
            break;
        }
        case 8:
        {
            BigInt first = BigIntInput();
            if (first.sign == 1)
            {
                printf("Square root of negative number is not defined\n");
            }
            else
            {
                BigInt ans = BigIntSqrtInt(first);
                printBigInt(ans);
            }
            break;
        }
        default:
            printf("Invalid choice\n");
        }
        printf("\n\n\nEnter choice:->\n 1)Add 2 BigInt(positive only) \n 2)Subtract 2 BigInt(positive only)\n 3)Multiply 2 BigInt\n 4)Divide BigInt by long\n 5)Modulo of 2 BigInt(Positive Only)\n 6)Power of BigInt to positive integer\n 7)Factorial of BigInt\n 8)Sqrt of BigInt\n 0)Exit Program\n\n\n");
        scanf("%d", &choice);
    }

    return 0;
}
