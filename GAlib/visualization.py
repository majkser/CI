import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import os

def plot_square_and_circle(square, circle_center, circle_radius, filename="square_circle.png"):
    x, y = zip(*square)
    fig, ax = plt.subplots()

    ax.fill(x + (x[0],), y + (y[0],), edgecolor='blue', fill=False, linewidth=2, label='square')

    circle = plt.Circle(circle_center, circle_radius, color='red', fill=False, linewidth=2, label='Inscribed Circle')
    ax.add_artist(circle)

    ax.set_aspect('equal', adjustable='box')

    padding = circle_radius * 0.1
    ax.set_xlim(min(x) - padding, max(x) + padding)
    ax.set_ylim(min(y) - padding, max(y) + padding)
    ax.legend()

    plt.title('square and Inscribed Circle')
    plt.xlabel('X-axis')
    plt.ylabel('Y-axis')
    plt.grid(True)
    plt.plot()
    plt.savefig(filename)
    plt.close()
    
def plot_fitness_evolution(filename="fitness_evolution.png"):
    if os.path.exists("ga_stats.csv"):
        df = pd.read_csv("ga_stats.csv")
        
        fig, ax = plt.subplots(figsize=(10, 6))
        
        ax.plot(df['Generation'], df['BestFitness'], label='Best Fitness', color='green', linewidth=2)
        ax.plot(df['Generation'], df['AvgFitness'], label='Average Fitness', color='blue', linewidth=2)
        
        ax.set_xlabel('Generation')
        ax.set_ylabel('Fitness')
        ax.set_title('Fitness Evolution over Generations')
        ax.legend()
        ax.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig(filename)
        plt.close()
        
        print(f"Fitness evolution plot saved to {filename}")
    else:
        print("Error: ga_stats.csv not found")
    
if __name__ == "__main__":
    
    if os.path.exists("best_circle.csv"):
        df = pd.read_csv("best_circle.csv")
        square = [
            (df['x0'].iloc[0], df['y0'].iloc[0]),
            (df['x1'].iloc[0], df['y1'].iloc[0]),
            (df['x2'].iloc[0], df['y2'].iloc[0]),
            (df['x3'].iloc[0], df['y3'].iloc[0])
        ] 
        
        circle_center = (df['cx'].iloc[0], df['cy'].iloc[0])
        circle_radius = (df['r']).iloc[0]
        
        print(f"square: {square}")
        print (f"Circle Center: {circle_center}, Circle Radius: {circle_radius}")

        plot_square_and_circle(square, circle_center, circle_radius)
        plot_fitness_evolution()