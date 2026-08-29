#include <stdio.h>

int main()
{
    int arr[] = {7, 8, 2, 1, 4};
    int n = sizeof(arr)/ sizeof(arr[0]);

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n - 1; j++)
        {
            if(arr[j] > arr[j + 1])
            {
                int temp_hold = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp_hold;
            }
        }
    }

    for(int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");

    return 0;
}