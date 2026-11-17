import fs from "fs";import {csv} from "../services/engineService.js";
export function records(){if(!fs.existsSync(csv))return [];const lines=fs.readFileSync(csv,"utf8").trim().split(/\r?\n/).slice(1);return lines.map(l=>{const c=l.split(",");return {timestamp:+c[0],image:c[1],width:+c[2],height:+c[3],pixels:+c[4],mode:c[5],threads:+c[6],elapsed_ms:+c[7]};}).filter(x=>Number.isFinite(x.elapsed_ms));}
export function getAll(_,res){res.json({success:true,records:records()});}
export function summary(_,res){const a=records(),p=a.filter(x=>x.mode==="parallel"),t=p.map(x=>x.elapsed_ms);res.json({success:true,total_runs:a.length,parallel_runs:p.length,best_record_ms:t.length?Math.min(...t):null,average_parallel_ms:t.length?t.reduce((x,y)=>x+y,0)/t.length:null,records:a.slice(-10)});}
