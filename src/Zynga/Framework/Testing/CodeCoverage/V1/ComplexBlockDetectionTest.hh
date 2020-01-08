<?hh // strict

namespace Zynga\Framework\Testing\CodeCoverage\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Testing\CodeCoverage\V1\ComplexBlockDetection;

class ComplexBlockDetectionTest extends ZyngaTestCase {

  public function testSimpleIfs(): void {

    $obj = new ComplexBlockDetection();
    $this->assertTrue($obj->simpleIf('bar'));
    $this->assertFalse($obj->simpleIf('baz'));

  }

  public function testComplexIfs(): void {

    $obj = new ComplexBlockDetection();
    $this->assertTrue($obj->moreComplexIf('bar'));
    $this->assertTrue($obj->moreComplexIf('baz'));
    $this->assertFalse($obj->moreComplexIf('jeo'));

  }

  public function testSimpleTryCatch(): void {

    $obj = new ComplexBlockDetection();
    $this->assertTrue($obj->simpleTryCatch('bar'));
    $this->assertFalse($obj->simpleTryCatch('baz'));

  }

  public function testComplexTryCatch(): void {

    $obj = new ComplexBlockDetection();

    $this->assertTrue($obj->moreComplexTryCatch('bar'));
    $this->assertTrue($obj->didFinally());

    $obj->resetFinally();
    $this->assertFalse($obj->moreComplexTryCatch('baz'));
    $this->assertTrue($obj->didFinally());

  }

  public function testPrivateDryRun(): void {

    $obj = new ComplexBlockDetection();
    $this->assertTrue($obj->pokePrivateWithIsDryRun());
    $this->assertFalse($obj->pokePrivateWithIsNotDryRun());

  }

}
