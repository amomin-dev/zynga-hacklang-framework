<?hh //strict

namespace Zynga\Framework\Cache\V2\Config\LocalMemcache\Mock\NoServersConfigured;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use
  Zynga\Framework\Cache\V2\Config\LocalMemcache\Mock\NoServersConfigured\Dev as ConfigUnderTest
;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject
;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidExampleObjectRequiredFields
;

class DevTest extends TestCase {

  public function createConfigUnderTest(): ConfigUnderTest {
    return new ConfigUnderTest();
  }

  public function testConfig_Settings(): void {

    $config = $this->createConfigUnderTest();

    $this->assertTrue($config->cacheAllowsKeyOverride());
    $this->assertTrue($config->cacheAllowsNonExpiringKeys());
    $this->assertTrue($config->cacheAllowsTTLOverride());

    $this->assertEquals('Memcache', $config->getDriver());
    $this->assertEquals(3600, $config->getTTL());

  }

  public function testGetServerPairings(): void {

    $config = $this->createConfigUnderTest();
    $servers = $config->getServerPairings();
    $this->assertEquals(0, $servers->keys()->count());

  }

  public function testCreateKeyFromStorableObject_ExceptionWired(): void {

    $obj = new ValidExampleObjectRequiredFields();

    $config = $this->createConfigUnderTest();

    $this->expectException(InvalidObjectForKeyCreationException::class);
    $key = $config->createKeyFromStorableObject($obj);

  }

}
