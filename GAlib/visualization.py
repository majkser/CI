import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import os

def plot_square_and_circle(square, circle_center, circle_radius, filename="square_circle.png"):
    """
    Plots a square and its inscribed circle, then saves the plot to a file.

    Parameters:
    - square: List of (x, y) tuples representing the vertices of the square.
    - circle_center: (x, y) tuple representing the center of the inscribed circle.
    - circle_radius: Radius of the inscribed circle.
    - filename: Name of the file to save the plot.
    """
    # Unzip square vertices
    x, y = zip(*square)

    # Create figure and axis
    fig, ax = plt.subplots()

    # Plot square
    ax.fill(x + (x[0],), y + (y[0],), edgecolor='blue', fill=False, linewidth=2, label='square')

    # Plot inscribed circle
    circle = plt.Circle(circle_center, circle_radius, color='red', fill=False, linewidth=2, label='Inscribed Circle')
    ax.add_artist(circle)

    # Set equal aspect ratio
    ax.set_aspect('equal', adjustable='box')

    # Set limits
    padding = circle_radius * 0.1
    ax.set_xlim(min(x) - padding, max(x) + padding)
    ax.set_ylim(min(y) - padding, max(y) + padding)

    # Add legend
    ax.legend()

    #plot 
    plt.title('square and Inscribed Circle')
    plt.xlabel('X-axis')
    plt.ylabel('Y-axis')
    plt.grid(True)
    plt.plot()
    plt.savefig(filename)
    plt.close()
    
if __name__ == "__main__":
    if os.path.exists("best_circle.csv"):
        # Load square data from CSV
        df = pd.read_csv("best_circle.csv")
        square = [
            (df['x0'].iloc[0], df['y0'].iloc[0]),
            (df['x1'].iloc[0], df['y1'].iloc[0]),
            (df['x2'].iloc[0], df['y2'].iloc[0]),
            (df['x3'].iloc[0], df['y3'].iloc[0])
        ] # Example square vertices
        
        # Example inscribed circle parameters
        circle_center = (df['cx'].iloc[0], df['cy'].iloc[0])
        circle_radius = (df['r']).iloc[0]
        
        print(f"square: {square}")
        print (f"Circle Center: {circle_center}, Circle Radius: {circle_radius}")
        # Plot and save
        plot_square_and_circle(square, circle_center, circle_radius)