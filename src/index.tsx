import { NativeModules } from 'react-native';

  //@ts-ignore
const simpleJsiModule: {
  helloWorld(): string;
  multiply(x: number, y: number): number;
  //@ts-ignore
} = global;

export function isLoaded() {
  return typeof simpleJsiModule.multiply === 'function';
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
