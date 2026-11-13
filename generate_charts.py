import os
import matplotlib
os.environ['MPLCONFIGDIR'] = os.path.join(os.getcwd(), 'tmp_mpl')
import matplotlib.pyplot as plt
import numpy as np

# Set font styling
plt.rcParams['font.sans-serif'] = 'DejaVu Sans'
plt.rcParams['font.family'] = 'sans-serif'

# -------------------------------------------------------------
# FIGURE 1: Search Comparisons (Successful vs Unsuccessful)
# -------------------------------------------------------------
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 4.4), dpi=300)

# Colors matching the reference PDF:
# Red for BST, Blue for AVL, Green for Hash Table
color_bst = '#C0392B'   # Dark Red
color_avl = '#2980B9'   # Dark Blue
color_hash = '#27AE60'  # Green

# (a) Successful Search
keys_succ = ['1005\n(Root/L)', '1200\n(Mid)', '1650\n(Mid-R)', '2280\n(Deep)', '3100\n(Leaf)', 'MEAN']
bst_succ = [4, 5, 12, 19, 25, 13.0]
avl_succ = [5, 2, 6, 6, 6, 5.0]
hash_succ = [1, 1, 1, 1, 2, 1.2]

x = np.arange(len(keys_succ))
width = 0.25

rects1 = ax1.bar(x - width, bst_succ, width, label='BST', color=color_bst)
rects2 = ax1.bar(x, avl_succ, width, label='AVL Tree', color=color_avl)
rects3 = ax1.bar(x + width, hash_succ, width, label='Hash Table (Linear Probe)', color=color_hash)

ax1.set_title('(a) Successful Search (Record Present)', fontsize=11, fontweight='bold', pad=10)
ax1.set_ylabel('Key Comparisons / Slot Checks', fontsize=10, fontweight='bold')
ax1.set_xticks(x)
ax1.set_xticklabels(keys_succ, fontsize=8.5)
ax1.set_ylim(0, 32)
ax1.grid(axis='y', linestyle='--', alpha=0.4)
ax1.legend(loc='upper left', fontsize=8.5, framealpha=0.9)

# Add values above bars
def autolabel(rects, ax):
    for rect in rects:
        h = rect.get_height()
        val_str = f'{h:.1f}' if isinstance(h, float) and not h.is_integer() else f'{int(h)}'
        ax.annotate(val_str,
                    xy=(rect.get_x() + rect.get_width() / 2, h),
                    xytext=(0, 2),  # 2 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom', fontsize=7.5, fontweight='bold')

autolabel(rects1, ax1)
autolabel(rects2, ax1)
autolabel(rects3, ax1)

# (b) Unsuccessful Search
keys_unsucc = ['9999\n(>Max)', '1000\n(<Min)', '1505\n(Mid-1)', '2222\n(Mid-2)', '3500\n(Tail)', 'MEAN']
bst_unsucc = [25, 4, 11, 18, 25, 16.6]
avl_unsucc = [6, 5, 6, 6, 6, 5.8]
hash_unsucc = [1, 1, 3, 5, 1, 2.2]

x2 = np.arange(len(keys_unsucc))

rects4 = ax2.bar(x2 - width, bst_unsucc, width, label='BST', color=color_bst)
rects5 = ax2.bar(x2, avl_unsucc, width, label='AVL Tree', color=color_avl)
rects6 = ax2.bar(x2 + width, hash_unsucc, width, label='Hash Table (Linear Probe)', color=color_hash)

ax2.set_title('(b) Unsuccessful Search (Record Absent)', fontsize=11, fontweight='bold', pad=10)
ax2.set_ylabel('Key Comparisons / Slot Checks', fontsize=10, fontweight='bold')
ax2.set_xticks(x2)
ax2.set_xticklabels(keys_unsucc, fontsize=8.5)
ax2.set_ylim(0, 32)
ax2.grid(axis='y', linestyle='--', alpha=0.4)
ax2.legend(loc='upper left', fontsize=8.5, framealpha=0.9)

autolabel(rects4, ax2)
autolabel(rects5, ax2)
autolabel(rects6, ax2)

plt.tight_layout()
fig.savefig('figure1.png', dpi=300)
plt.close(fig)
print('figure1.png generated successfully')

# -------------------------------------------------------------
# FIGURE 2: Tree Height & Hash Placement Distribution
# -------------------------------------------------------------
fig2, (ax3, ax4) = plt.subplots(1, 2, figsize=(11, 4.4), dpi=300)

# (a) Tree Height Comparison
labels_h = ['Optimal Bound\nceil(log2(N+1))', 'AVL Tree\n(Balanced)', 'BST\n(Skewed)', 'Degenerate\nChain (N)']
heights = [6, 6, 25, 40]
colors_h = ['#2980B9', '#2980B9', '#C0392B', '#7F8C8D']

bars_h = ax3.bar(labels_h, heights, width=0.55, color=colors_h)
ax3.set_title('(a) Tree Height Comparison (N = 40)', fontsize=11, fontweight='bold', pad=10)
ax3.set_ylabel('Height (Levels)', fontsize=10, fontweight='bold')
ax3.set_ylim(0, 48)
ax3.grid(axis='y', linestyle='--', alpha=0.4)
ax3.axhline(6, color='blue', linestyle=':', alpha=0.7, label='Balanced Height (H=6)')
ax3.legend(loc='upper left', fontsize=8.5)

for bar in bars_h:
    h = bar.get_height()
    ax3.annotate(f'{int(h)}',
                 xy=(bar.get_x() + bar.get_width() / 2, h),
                 xytext=(0, 3),
                 textcoords="offset points",
                 ha='center', va='bottom', fontsize=8.5, fontweight='bold')

# (b) Hash Placement Distribution
probe_categories = ['Direct (0)', '+1 Probe', '+2 Probes', '+3 Probes', '+4 Probes', '>=5 Probes']
probe_counts = [35, 4, 1, 0, 0, 0]
percentages = [87.5, 10.0, 2.5, 0.0, 0.0, 0.0]

bars_p = ax4.bar(probe_categories, probe_counts, width=0.55, color='#27AE60')
ax4.set_title('(b) Hash Table Placement Distribution (M = 53, N = 40, α = 75.5%)', fontsize=10.5, fontweight='bold', pad=10)
ax4.set_ylabel('Number of Records', fontsize=10, fontweight='bold')
ax4.set_ylim(0, 42)
ax4.grid(axis='y', linestyle='--', alpha=0.4)

for bar, pct in zip(bars_p, percentages):
    h = bar.get_height()
    if h > 0:
        ax4.annotate(f'{int(h)} ({pct:.1f}%)',
                     xy=(bar.get_x() + bar.get_width() / 2, h),
                     xytext=(0, 3),
                     textcoords="offset points",
                     ha='center', va='bottom', fontsize=8, fontweight='bold')

# Annotation callout box for primary clustering
ax4.annotate('Controlled Probing\n6 Initial Collisions\nMax Offset = +2 Slots',
             xy=(1, 4), xytext=(2.2, 25),
             arrowprops=dict(arrowstyle="->", color='#C0392B', lw=1.5),
             bbox=dict(boxstyle="round,pad=0.5", fc="#FDEDEC", ec="#C0392B", lw=1),
             fontsize=8.5, fontweight='bold', color='#C0392B')

plt.tight_layout()
fig2.savefig('figure2.png', dpi=300)
plt.close(fig2)
print('figure2.png generated successfully')
