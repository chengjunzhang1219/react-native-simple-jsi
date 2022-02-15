/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow strict-local
 */

import React from 'react';
import {StyleSheet, Text, View} from 'react-native';

const App = () => {
  const [result, setResult] = React.useState<number | undefined>();

  React.useEffect(() => {
    setResult(global.helloWorld());
  }, []);

  return (
    <View style={styles.container}>
      <Text style={styles.result}>Result: {result}</Text>
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    paddingHorizontal: 24,
    alignItems: 'center',
    justifyContent: 'center',
  },
  result: {
    color: 'black',
    fontSize: 20,
  },
});

export default App;
