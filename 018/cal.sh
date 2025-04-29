#!/bin/bash


# Simple Calculator Script

while true; do
    echo "--------------------------"
    echo "  Basic Calculator Menu   "
    echo "--------------------------"
    echo "1. Addition"
    echo "2. Subtraction"
    echo "3. Multiplication"
    echo "4. Division"
    echo "5. Modulus"
    echo "6. Exit"
    echo "--------------------------"
    read -p "Enter your choice (1-6): " choice

    case $choice in
        1)
            read -p "Enter first number: " num1
            read -p "Enter second number: " num2
            echo "$num1 + $num2 = $(($num1 + $num2))"
            ;;
        2)
            read -p "Enter first number: " num1
            read -p "Enter second number: " num2
            echo "$num1 - $num2 = $(($num1 - $num2))"
            ;;
        3)
            read -p "Enter first number: " num1
            read -p "Enter second number: " num2
            echo "$num1 * $num2 = $(($num1 * $num2))"
            ;;
        4)
            read -p "Enter first number: " num1
            read -p "Enter second number: " num2
            if [ $num2 -eq 0 ]; then
                echo "Error: Division by zero is not allowed!"
            else
                echo "$num1 / $num2 = $(echo "scale=2; $num1 / $num2" | bc)"
            fi
            ;;
        5)
            read -p "Enter first number: " num1
            read -p "Enter second number: " num2
            if [ $num2 -eq 0 ]; then
                echo "Error: Modulus by zero is not allowed!"
            else
                echo "$num1 % $num2 = $(($num1 % $num2))"
            fi
            ;;
        6)
            echo "Exiting calculator. Goodbye!"
            exit 0
            ;;
        *)
            echo "Invalid choice! Please enter a number between 1 and 6."
            ;;
    esac
    
    read -p "Press any key to continue..."
    clear
done