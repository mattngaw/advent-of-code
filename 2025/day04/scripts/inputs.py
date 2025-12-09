import random
import argparse

def write_random_square(size, filename="square.txt", at_probability=0.6):
    """
    Writes a text file containing a square grid of random '@' and '.' characters.

    Parameters:
        size (int): Width/height of the square.
        filename (str): File to write output into.
        at_probability (float): Probability of generating '@' (0.0 to 1.0).
    """
    with open(filename, "w") as f:
        for _ in range(size):
            line = "".join('@' if random.random() < at_probability else '.' 
                           for _ in range(size))
            f.write(line + "\n")

    print(f"Square ({size}×{size}) written to {filename}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate a square of random '@' and '.' characters.")
    parser.add_argument("size", type=int, help="Size of the square (e.g., 20 for a 20x20 grid)")
    parser.add_argument("--file", default="square.txt", help="Output filename (default: square.txt)")
    parser.add_argument("--p", type=float, default=0.6,
                        help="Probability of '@' (0.0–1.0, default: 0.6)")

    args = parser.parse_args()

    write_random_square(size=args.size, filename=args.file, at_probability=args.p)
