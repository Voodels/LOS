# Function to check if a number is prime
def is_prime(n, i=2):
    if n <= 2:
        return n == 2
    if n % i == 0:
        return False
    if i * i > n:
        return True
    return is_prime(n, i + 1)

# Recursive function to print primes up to 'limit'
def print_primes(limit, current=2):
    if current > limit:
        return
    if is_prime(current):
        print(current, end=' ')
    print_primes(limit, current + 1)

# Driver Code
limit = int(input("Enter the limit: "))
print_primes(limit)
