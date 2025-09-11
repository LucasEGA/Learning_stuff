public class Main{
    static int binary_search(int numbers[], int target)
    {
        int left = 0;
        int right = numbers.length - 1; 
        while (left <= right)
        {
             int mid = (left + right) / 2;
            if (numbers[mid] == target)
            {
                return mid;
            }
             if (numbers[mid] > target)
            {
                right = mid - 1;
            }
            else
            {
            left = mid + 1 ;    
            } 
        }
        return -1;
    }
}
