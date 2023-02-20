def H(x):
    r = 0
    while x:
        r += 1
        x &= x - 1
    return r
def correlation(A, B):
    x = sum(A) / len(A)
    y = sum(B) / len(B)
    return sum((a - x) * (b - y) for a, b in zip(A, B)) / (sum((a - x) ** 2 for a in A) * sum((b - y) ** 2 for b in B)) ** 0.5
input = [0, 1, 2, 3]
power = [2, 0, 1, 1]
S_box = [1, 2, 3, 0]
corrs = [correlation([H(S_box[i ^ key]) for i in input], power) for key in range(4)]
print("Correlation: ", corrs)
print("Correct Key(s): ", [i for i, x in enumerate(corrs) if abs(x) == max(abs(x) for x in corrs)])
