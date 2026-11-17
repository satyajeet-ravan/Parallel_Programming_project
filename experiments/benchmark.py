import argparse,csv,json,platform,subprocess,time
from pathlib import Path
import cv2,numpy as np
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent
DEFAULT=ROOT/"cpp-engine"/"build"/("image_engine.exe" if platform.system()=="Windows" else "image_engine")
def main():
 p=argparse.ArgumentParser();p.add_argument("--engine",type=Path,default=DEFAULT);p.add_argument("--repeats",type=int,default=3);p.add_argument("--sizes",default="500x500,1000x1000,2000x2000,4000x3000");p.add_argument("--threads",default="1,2,4,8");a=p.parse_args()
 if not a.engine.exists():raise SystemExit(f"Engine not found: {a.engine}; build it first.")
 outcsv=HERE/"results.csv";fields=["timestamp","image","width","height","pixels","mode","threads","elapsed_ms","cpu","platform"];new=not outcsv.exists()
 with outcsv.open("a",newline="") as f:
  wri=csv.DictWriter(f,fieldnames=fields)
  if new:wri.writeheader()
  for size in a.sizes.split(","):
   width,height=map(int,size.split("x"));rng=np.random.default_rng(width+height+2026);img=rng.integers(0,256,(height,width,3),dtype=np.uint8);src=HERE/f"test_{size}.png";dst=HERE/f"out_{size}.png";cv2.imwrite(str(src),img)
   for t in map(int,a.threads.split(",")):
    for rep in range(a.repeats):
     cmd=[str(a.engine),"--input",str(src),"--output",str(dst),"--mode","parallel","--threads",str(t),"--benchmark",str(outcsv)]
     run=subprocess.run(cmd,text=True,capture_output=True)
     if run.returncode:raise RuntimeError(run.stderr)
     result=json.loads(run.stdout);row={"timestamp":int(time.time()),"image":src.name,"width":width,"height":height,"pixels":width*height,"mode":"parallel","threads":t,"elapsed_ms":result["elapsed_ms"],"cpu":platform.processor() or platform.machine(),"platform":platform.platform()}
     wri.writerow(row);f.flush();print(f"{size} t={t} run={rep+1}: {row['elapsed_ms']:.3f} ms")
 print("Recorded actual measurements in",outcsv)
if __name__=="__main__":main()
