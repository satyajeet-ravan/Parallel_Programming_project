import {spawn} from "child_process";import fs from "fs";import path from "path";import {fileURLToPath} from "url";
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),"../..");
export const inputDir=path.join(root,"images/input"),outputDir=path.join(root,"images/output"),csv=path.join(root,"experiments/results.csv");
export const engine=process.env.IMAGE_ENGINE_PATH||path.join(root,"cpp-engine/build",process.platform==="win32"?"image_engine.exe":"image_engine");
[inputDir,outputDir,path.dirname(csv)].forEach(p=>fs.mkdirSync(p,{recursive:true}));
export function runEngine(o){return new Promise((resolve,reject)=>{if(!fs.existsSync(engine))return reject(Error("C++ engine missing: "+engine+"; build cpp-engine first."));
 const args=["--input",o.input,"--output",o.output,"--mode",o.mode,"--threads",String(o.threads),"--benchmark",csv];const p=spawn(engine,args,{cwd:root,windowsHide:true});let out="",err="";
 p.stdout.on("data",d=>out+=d);p.stderr.on("data",d=>err+=d);p.on("error",reject);p.on("close",code=>{if(code!==0)return reject(Error(err||"Engine exited "+code));try{resolve({...JSON.parse(out.trim()),diagnostics:err});}catch{reject(Error("Invalid engine JSON: "+out+" "+err));}});
});}
