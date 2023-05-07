import { appDataDir } from '@tauri-apps/api/path';
import {
  createDir,
  exists,
  readTextFile,
  writeTextFile
} from '@tauri-apps/api/fs';
import type { GameRecord } from '~/types';

export async function ensureDirectory() {
  const appDataBaseDir = await appDataDir();
  for (const x of ['records']) {
    await createDir(`${appDataBaseDir}${x}`, { recursive: true });
  }
}

export async function writeRecords(levelName: string, records: GameRecord[]) {
  const filePath = `${await appDataDir()}records/${levelName}.json`;
  return await writeTextFile(
    filePath,
    JSON.stringify(records.sort((a, b) => b.score - a.score))
  );
}

export async function readRecords(levelName: string) {
  const filePath = `${await appDataDir()}records/${levelName}.json`;
  if (!(await exists(filePath))) {
    await writeRecords(levelName, []);
  }
  return JSON.parse(await readTextFile(filePath)) as GameRecord[];
}

export async function addRecord(levelName: string, record: GameRecord) {
  const records = await readRecords(levelName);
  records.push(record);
  await writeRecords(levelName, records);
}
