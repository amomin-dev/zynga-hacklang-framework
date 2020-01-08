<?hh // strict

namespace Zynga\Framework\Type\V1\Test;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\ValidValue;
use Zynga\Framework\Type\V1\Exceptions\UnSupportedTypeException;

abstract class TestCase extends ZyngaTestCase {

  abstract public function getTypeObject(): TypeInterface;

  abstract public function generateValidValues(): Vector<ValidValue>;

  abstract public function generateInvalidValues(): Vector<mixed>;

  public function test_requiredToggle(): void {
    $obj = $this->getTypeObject();

    // Default is off
    $this->assertFalse($obj->getIsRequired());

    // Turn it on
    $this->assertTrue($obj->setIsRequired(true));
    $this->assertTrue($obj->getIsRequired());

    // Turn it off
    $this->assertTrue($obj->setIsRequired(false));
    $this->assertFalse($obj->getIsRequired());

  }

  public function testInvalidType_Import(): void {
    $e = new Exception('BreakingStuff');
    $obj = $this->getTypeObject();
    $this->expectException(UnSupportedTypeException::class);
    $obj->set($e);
  }

  public function testValidValues(): void {

    $validValues = $this->generateValidValues();

    // --
    // Every type check should have some version of a set of data that is good
    // for it.
    // --
    if ($validValues->count() == 0) {
      $this->fail('all type check tests should have valid values');
    }

    foreach ($validValues as $validValue) {
      // do it in the native test type, if you need to test a string version
      // of a value add it to your valid values set.
      $obj = $this->getTypeObject();
      $this->assertTrue($obj->set($validValue->getInputValue()));
      $this->assertEquals($validValue->getExpectedValue(), $obj->get());
    }

    foreach ($validValues as $validValue) {
      $obj = $this->getTypeObject();
      $this->assertTrue($obj->setDefaultValue($validValue->getInputValue()));
      list($isDefaultValue, $messages) = $obj->isDefaultValue();
      $this->assertTrue($isDefaultValue);
      $this->assertEquals($validValue->getExpectedValue(), $obj->get());
    }

  }

  public function doTestInvalidValues_Against_Set(
    Vector<mixed> $invalidValues,
  ): void {
    foreach ($invalidValues as $invalidValue) {
      // do it in the native test type first.
      try {
        $obj = $this->getTypeObject();
        $obj->set($invalidValue);
        $this->fail(
          'value='.strval($invalidValue).' should of been rejected',
        );
      } catch (Exception $e) {
        $this->assertTrue(true);
      }
    }
  }

  public function doTestInvalidValues_Against_Strval(
    Vector<mixed> $invalidValues,
  ): void {

    foreach ($invalidValues as $invalidValue) {

      // now convert the value into a string, and test again.
      try {
        $stringValue = strval($invalidValue);
        $obj = $this->getTypeObject();
        $obj->set($stringValue);
        $this->fail('value='.$stringValue.' should of been rejected for set');
      } catch (Exception $e) {
        $this->assertTrue(true);
      }

    }
  }

  public function doTestInvalidValues_Against_SetDefaultValue(
    Vector<mixed> $invalidValues,
  ): void {

    foreach ($invalidValues as $invalidValue) {
      // do it in the native test type first.
      try {
        $obj = $this->getTypeObject();
        $obj->setDefaultValue($invalidValue);
        $this->fail(
          'value='.
          strval($invalidValue).
          ' should of been rejected for setDefaultValue',
        );
      } catch (Exception $e) {
        $this->assertTrue(true);
      }
    }

  }

  public function testInvalidValues(): void {

    $invalidValues = $this->generateInvalidValues();

    // --
    // Every type check should have some version of a set of data that is bad
    // for it.
    // --
    if ($invalidValues->count() == 0) {
      $this->fail('all type check tests should have invalid values');
    }

    $this->doTestInvalidValues_Against_Set($invalidValues);
    $this->doTestInvalidValues_Against_Strval($invalidValues);
    $this->doTestInvalidValues_Against_SetDefaultValue($invalidValues);

  }

  protected function createInjectedValues(
    Vector<string> $badData,
  ): Vector<string> {

    $injectedData = Vector {};

    $validValues = $this->generateValidValues();

    foreach ($validValues as $validValue) {

      $inputData = $validValue->getInputValue();

      $testValue = strval($inputData);

      foreach ($badData as $bad) {
        $testString = $testValue.$bad;
        $injectedData->add($testString);
      }

    }
    return $injectedData;
  }

  protected function injectionTestScaffold(Vector<string> $badData): void {

    $injectedData = $this->createInjectedValues($badData);

    foreach ($injectedData as $injected) {
      $obj = $this->getTypeObject();
      $this->expectException(Exception::class);
      $obj->set($injected);
    }

  }

  public function testFacebookTagInjection(): void {
    $badData = Vector {};
    $badData->add('<fb:');
    $this->injectionTestScaffold($badData);
  }

  public function testImgTagInjection(): void {
    $badData = Vector {};
    $badData->add('<img');
    $this->injectionTestScaffold($badData);
  }

  public function testOnLoadInjection(): void {
    $badData = Vector {};
    $badData->add(
      "<tag src=\"htttp://www.example.com/image.jpeg\" onload = \"javascript:alert('xss');\">",
    );
    $badData->add("onLoad='exploit'");
    $this->injectionTestScaffold($badData);
  }

  public function testScriptTagInjection(): void {
    $badData = Vector {};
    $badData->add('<script');
    $badData->add('<a href="script:');
    $badData->add('script=');
    $this->injectionTestScaffold($badData);
  }

  public function testOnFocusInjection(): void {
    $badData = Vector {};
    $badData->add('<img onfocus="script:');
    $badData->add('<obj onFocus=');
    $this->injectionTestScaffold($badData);
  }

  public function testIFrameTagInjection(): void {
    $badData = Vector {};
    $badData->add('<iframe');
    $badData->add('<fb:iframe');
    $this->injectionTestScaffold($badData);
  }

  public function testFromCharCodeInjection(): void {
    $badData = Vector {};
    $badData->add('String.fromCharCode(65, 66, 67);');
    $badData->add(urlencode('String.fromCharCode(65, 66, 67);'));
    $this->injectionTestScaffold($badData);
  }

  public function testReset(): void {

    $obj = $this->getTypeObject();

    $this->assertTrue($obj->reset());

  }

}
