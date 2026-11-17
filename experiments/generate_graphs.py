import argparse
from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt
HERE=Path(__file__).resolve().parent
def main():
 p=argparse.ArgumentParser();p.add_argument("--csv",type=Path,default=HERE/"results.csv");a=p.parse_args()
 if not a.csv.exists():raise SystemExit("Run benchmark.py first; no results.csv found.")
 d=pd.read_csv(a.csv);d=d.dropna(subset=["elapsed_ms"]);d=d[d.elapsed_ms>0];out=HERE/"graphs";out.mkdir(exist_ok=True)
 q=d[d.mode=="parallel"]
 if q.empty:raise SystemExit("No parallel measurement rows.")
 g=q.groupby(["pixels","threads"],as_index=False).elapsed_ms.mean()
 for px,x in g.groupby("pixels"):
  plt.figure();plt.plot(x.threads,x.elapsed_ms,marker="o");plt.xlabel("Threads");plt.ylabel("Average time (ms)");plt.title(f"Execution time · {int(px)} pixels");plt.grid(True);plt.savefig(out/f"time_{int(px)}.png",dpi=160);plt.close()
 # Speedup/efficiency only when genuine sequential baseline rows exist.
 seq=d[d.mode=="sequential"].groupby("pixels").elapsed_ms.mean().to_dict();rows=[]
 for _,r in g.iterrows():
  if r.pixels in seq:rows.append({"pixels":r.pixels,"threads":r.threads,"speedup":seq[r.pixels]/r.elapsed_ms,"efficiency":seq[r.pixels]/r.elapsed_ms/r.threads*100})
 if rows:
  m=pd.DataFrame(rows)
  for col in ["speedup","efficiency"]:
   plt.figure()
   for px,x in m.groupby("pixels"):plt.plot(x.threads,x[col],marker="o",label=str(int(px)))
   plt.xlabel("Threads");plt.ylabel(col);plt.legend();plt.grid(True);plt.savefig(out/f"{col}.png",dpi=160);plt.close()
 print("Graphs saved:",out)
if __name__=="__main__":main()
