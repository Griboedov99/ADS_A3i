import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def plot_intro_sort_results():
    # Read data
    quick_data = pd.read_csv('quick_sort_results.csv')
    intro_data = pd.read_csv('intro_sort_results.csv')
    components_data = pd.read_csv('components_analysis.csv')

    # Create subplots
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(16, 12))

    # Plot 1: Standard Quick Sort performance
    ax1.plot(quick_data['Size'], quick_data['Random'], label='Random')
    ax1.plot(quick_data['Size'], quick_data['Reverse'], label='Reverse Sorted', linestyle='--')
    ax1.plot(quick_data['Size'], quick_data['NearlySorted'], label='Nearly Sorted')
    ax1.plot(quick_data['Size'], quick_data['ManyDuplicates'], label='Many Duplicates', linestyle='-.')
    ax1.set_xlabel('Array Size')
    ax1.set_ylabel('Time (microseconds)')
    ax1.set_title('Standard Quick Sort Performance')
    ax1.legend()
    ax1.grid(True, alpha=0.3)

    # Plot 2: IntroSort vs QuickSort for worst case (reverse sorted)
    ax2.plot(quick_data['Size'], quick_data['Reverse'], label='Quick Sort', linewidth=2)
    ax2.plot(intro_data['Size'], intro_data['Reverse'], label='IntroSort', linewidth=2)
    ax2.set_xlabel('Array Size')
    ax2.set_ylabel('Time (microseconds)')
    ax2.set_title('Worst Case: Reverse Sorted Arrays')
    ax2.legend()
    ax2.grid(True, alpha=0.3)

    # Plot 3: Performance improvement
    improvement_random = (quick_data['Random'] - intro_data['Random']) / quick_data['Random'] * 100
    improvement_reverse = (quick_data['Reverse'] - intro_data['Reverse']) / quick_data['Reverse'] * 100
    improvement_nearly = (quick_data['NearlySorted'] - intro_data['NearlySorted']) / quick_data['NearlySorted'] * 100
    improvement_duplicates = (quick_data['ManyDuplicates'] - intro_data['ManyDuplicates']) / quick_data['ManyDuplicates'] * 100

    ax3.plot(quick_data['Size'], improvement_random, label='Random Arrays')
    ax3.plot(quick_data['Size'], improvement_reverse, label='Reverse Sorted', linewidth=2)
    ax3.plot(quick_data['Size'], improvement_nearly, label='Nearly Sorted')
    ax3.plot(quick_data['Size'], improvement_duplicates, label='Many Duplicates')
    ax3.set_xlabel('Array Size')
    ax3.set_ylabel('Performance Improvement (%)')
    ax3.set_title('IntroSort Performance Improvement Over QuickSort')
    ax3.legend()
    ax3.grid(True, alpha=0.3)

    # Plot 4: Component analysis
    ax4.plot(components_data['Size'], components_data['QuickSort'], label='QuickSort')
    ax4.plot(components_data['Size'], components_data['HeapSort'], label='HeapSort')
    ax4.plot(components_data['Size'], components_data['InsertionSort'], label='InsertionSort')
    ax4.plot(components_data['Size'], components_data['IntroSort'], label='IntroSort', linewidth=2)
    ax4.set_xlabel('Array Size')
    ax4.set_ylabel('Time (microseconds)')
    ax4.set_title('Algorithm Components Comparison')
    ax4.legend()
    ax4.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig('intro_sort_analysis.png', dpi=300, bbox_inches='tight')
    plt.show()

    # Print statistical analysis
    print("=== IntroSort vs QuickSort Performance Analysis ===")
    for col in ['Random', 'Reverse', 'NearlySorted', 'ManyDuplicates']:
        quick_avg = quick_data[col].mean()
        intro_avg = intro_data[col].mean()
        improvement = (quick_avg - intro_avg) / quick_avg * 100

        print(f"{col}:")
        print(f"  QuickSort avg: {quick_avg:.1f} μs")
        print(f"  IntroSort avg: {intro_avg:.1f} μs")
        print(f"  Improvement: {improvement:.1f}%")
        print()

if __name__ == "__main__":
    plot_intro_sort_results()