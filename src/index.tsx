import { NativeModules } from 'react-native';

//@ts-ignore
const simpleJsiModule: {
  helloWorld(): string;
  multiplyWithCallback(
    x: number,
    y: number,
    callback: (z: number) => void
  ): void;
  multiply(x: number, y: number): number;
  getDeviceName(): string;
  setItem(key: string, value: string): boolean;
  getItem(key: string): string;
  // setMessage(message: any): boolean;
  //@ts-ignore
} = global;

export function isLoaded() {
  return typeof simpleJsiModule.getItem === 'function';
}

if (!isLoaded()) {
  const result = NativeModules.RnJsiSecond.install();
  if (!result)
    throw new Error('JSI bindings were not installed for: SimpleJsi Module');

  if (!isLoaded()) {
    throw new Error('JSI bindings were not installed for: SimpleJsi Module');
  }
}
export default simpleJsiModule;
